//
// Created by thallock on 9/5/21.
//

#include <thread>
#include <iostream>
#include <ctime>

//#include "billiards_common/utils/Args.h"
#include "billiards_common/3rd_party/crow_all.h"
#include "billiards_common/utils/crow_common.h"
#include "billiards_common/config/ports.h"

#include "billiards_common/utils/SerializableCache.h"


#include "construct_map.h"
#include "front_end/FrontEnd.h"



namespace locals {

	class ThreadShared {
	public:
		int argc;
		char **argv;
		billiards::project::FrontEnd *front_end;
		billiards::json::SerializableCache<billiards::project::MappingInformation> cache;

		explicit ThreadShared(const std::string& mapping_loc)
			: argc{0}
			, argv{nullptr}
			, front_end{nullptr}
			, cache{mapping_loc}
		{}

		virtual ~ThreadShared() = default;
	};


	void run_front_end(ThreadShared *shared) {
		shared->front_end = billiards::project::create_front_end(shared->argc, shared->argv);
		if (shared->front_end == nullptr) {
			std::cerr << "Unable to create front end." << std::endl;
			return;
		}

		{
			std::lock_guard<std::mutex> guard{shared->front_end->mutex()};
			bool success = billiards::project::construct_map(shared->cache.cached, shared->front_end->display.map);
			if (!success) {
				std::cerr << "Unable to construct initial map" << std::endl;
//				return;
			}
		}

		shared->front_end->loop();
	}
}


int main(int argc, char **argv) {
	std::string mapping_loc{"./mapping.json"};
	for (int i = 0; i < argc - 1; i++) {
		if ("--mapping-file" == std::string{argv[i]} || "-f" == std::string{argv[i]}) {
			mapping_loc = argv[i+1];
			break;
		}
	}

	locals::ThreadShared shared{mapping_loc};
	shared.argc = argc;
	shared.argv = argv;
	shared.cache.reload();

	std::thread display_thread{locals::run_front_end, &shared};
	crow::SimpleApp app;

	DO_STATUS_ENDPOINT();

	CROW_ROUTE(app, "/mapping/")
		.methods("GET"_method, "PUT"_method, "OPTIONS"_method)
		([&](const crow::request& req) {
			if (req.method == "OPTIONS"_method) {
				HANDLE_OPTIONS;
			} else if (req.method == "GET"_method) {
				RETURN_SUCCESS_WITH_DATA(
						"Retrieved current mapping", "mapping", shared.cache.cached);
			} else if (req.method == "PUT"_method) {
				nlohmann::json value = nlohmann::json::parse(req.body);
				billiards::json::ParseResult result;

				billiards::config::TableDimensions dimensions;
				if (HAS_OBJECT(value, "mapping")) {
					shared.cache.cached.parse(value["mapping"], result);
				} else {
					RETURN_ERROR("No mapping found");
				}
				if (!result.success) {
					RETURN_ERROR("Unable to parse mapping");
				}
				bool success;
				{
					std::lock_guard<std::mutex> guard{shared.front_end->mutex()};
					success = billiards::project::construct_map(
							shared.cache.cached, shared.front_end->display.map);
				}
				if (!success) {
					RETURN_ERROR("Unable to construct map from mapping info");
				}
				if (!shared.cache.save()) {
					RETURN_ERROR("Unable to save new mapping");
				}
				shared.front_end->redraw();
				RETURN_SUCCESS("Updated the projector mapping");
			} else {
				return crow::response(404);
			}
		});

	CROW_ROUTE(app, "/graphics/")
		.methods("GET"_method, "PUT"_method, "OPTIONS"_method)
			([&](const crow::request& req) {
				if (req.method == "OPTIONS"_method) {
					HANDLE_OPTIONS;
				} else if (req.method == "GET"_method) {
					std::lock_guard<std::mutex> guard{shared.front_end->mutex()};
					RETURN_SUCCESS_WITH_DATA("Retrieved current graphics", "graphics",
						shared.front_end->display.graphics);
				} else if (req.method == "PUT"_method) {
					std::lock_guard<std::mutex> guard{shared.front_end->mutex()};
					nlohmann::json value = nlohmann::json::parse(req.body);
					billiards::json::ParseResult status;
					if (HAS_ARRAY(value, "graphics")) {
						shared.front_end->display.graphics.parse(value["graphics"], status);
					} else {
						RETURN_ERROR("No graphics provided");
					}
					if (!status.success) {
						RETURN_ERROR("Unable to parse graphics");
					}
					shared.front_end->redraw();
					RETURN_SUCCESS("Updated current graphics");
				} else {
					return crow::response(404);
				}
			});

	app.port(billiards::config::ports::PROJECTOR_API_PORT).run();

	shared.front_end->shutdown();
	display_thread.join();
	delete shared.front_end;
	return 0;
}
