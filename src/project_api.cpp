//
// Created by thallock on 9/5/21.
//

#include <pthread.h>
#include <iostream>

#include "common/utils/Args.h"

#include "common/3rd_party/crow_all.h"
#include "common/utils/crow_common.h"
#include "common/config/ports.h"

#include "front_end/FrontEnd.h"


namespace locals {
	billiards::project::FrontEnd *front_end;

	void *run_front_end(void *a) {
		auto *args = (billiards::utils::Args *) a;

		locals::front_end = billiards::project::create_front_end(args->argc, args->argv);
		if (locals::front_end == nullptr) {
			std::cerr << "Unable to create front end." << std::endl;
			return nullptr;
		}

		locals::front_end->loop();

		return nullptr;
	}
}


int main(int argc, char **argv) {
	billiards::utils::Args args{argc, argv};

	pthread_t display_thread;
	int error = pthread_create(&display_thread, nullptr, locals::run_front_end, (void *) &args);
	if (error) {
		std::cerr << "Unable to launch front end loop." << std::endl;
		return 1;
	}

	crow::SimpleApp app;

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
					RETURN_SUCCESS_WITH_DATA("Retrieved current graphics", "graphics",
						locals::front_end->display.graphics);
				} else if (req.method == "PUT"_method) {
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
	pthread_join(display_thread, nullptr);
	delete locals::front_end;
	return 0;
}
