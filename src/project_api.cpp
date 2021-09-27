//
// Created by thallock on 9/5/21.
//

#include <thread>
#include <iostream>
#include <ctime>

#include "common/utils/Args.h"

#include "common/3rd_party/crow_all.h"
#include "common/utils/crow_common.h"
#include "common/config/ports.h"

#include "front_end/FrontEnd.h"


namespace locals {
	billiards::project::FrontEnd *front_end;

	void run_front_end(int argc, char **argv) {
		int tmp_argc = argc;
		locals::front_end = billiards::project::create_front_end(tmp_argc, argv);
		if (locals::front_end == nullptr) {
			std::cerr << "Unable to create front end." << std::endl;
			return;
		}

		locals::front_end->loop();
	}

//	void *shutdown(void *) {
//		return nullptr;
//	}
}


int main(int argc, char **argv) {
	std::thread display_thread{locals::run_front_end, argc, argv};

	crow::SimpleApp app;

	DO_STATUS_ENDPOINT();

	CROW_ROUTE(app, "/location/")
		.methods("GET"_method, "PUT"_method, "OPTIONS"_method)
		([](const crow::request& req) {
			if (req.method == "OPTIONS"_method) {
				HANDLE_OPTIONS;
			} else if (req.method == "GET"_method) {
				RETURN_SUCCESS_WITH_DATA("Retrieved the current location", "location",
					locals::front_end->display.location);
			} else if (req.method == "PUT"_method) {
				nlohmann::json value = nlohmann::json::parse(req.body);

				if (value.contains("location") && value["location"].is_array()) {
					locals::front_end->display.location.parse(value["location"]);
				}
				locals::front_end->redraw();
				RETURN_SUCCESS("Updated the projector location");
			} else {
				return crow::response(404);
			}
		});

	CROW_ROUTE(app, "/graphics/")
		.methods("GET"_method, "PUT"_method, "OPTIONS"_method)
			([](const crow::request& req) {
				if (req.method == "OPTIONS"_method) {
					HANDLE_OPTIONS;
				} else if (req.method == "GET"_method) {
					std::lock_guard<std::mutex> guard{locals::front_end->mutex()};
					RETURN_SUCCESS_WITH_DATA("Retrieved current graphics", "graphics",
						locals::front_end->display.graphics);
				} else if (req.method == "PUT"_method) {
					std::lock_guard<std::mutex> guard{locals::front_end->mutex()};
					nlohmann::json value = nlohmann::json::parse(req.body);
					if (value.contains("graphics") && value["graphics"].is_array()) {
						locals::front_end->display.graphics.parse(value["graphics"]);
					}
					locals::front_end->redraw();
					RETURN_SUCCESS("Updated current graphics");
				} else {
					return crow::response(404);
				}
			});
	// Add graphics...

	app.port(billiards::config::ports::PROJECTOR_API_PORT).run();


	locals::front_end->shutdown();
	display_thread.join();
	delete locals::front_end;
	return 0;
}
