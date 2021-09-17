//
// Created by thallock on 9/5/21.
//

#include <pthread.h>
#include <iostream>

#include "common/3rd_party/crow_all.h"
#include "common/utils/dump.h"
#include "common/utils/Args.h"
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


#define RETURN_SUCCESS(msg) do {							\
	crow::json::wvalue x; 									\
	x["success"] = true;									\
	x["message"] = msg;             						\
    crow::response resp{x};									\
	resp.add_header("Access-Control-Allow-Origin", "*");	\
	return resp;			 								\
} while (false)



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
				crow::response resp;
				resp.add_header("Access-Control-Allow-Origin", "*");
				resp.add_header("Access-Control-Allow-Headers", "Content-Type");
				resp.add_header("Access-Control-Allow-Methods", "PUT, GET, OPTIONS");
				return resp;
			} else if (req.method == "GET"_method) {
				crow::response resp{billiards::json::dump(locals::front_end->display.location)};
				resp.add_header("Access-Control-Allow-Origin", "*");
				return resp;
			} else if (req.method == "PUT"_method) {
				nlohmann::json value = nlohmann::json::parse(req.body);
				locals::front_end->display.location.parse(value);
				locals::front_end->redraw();
				RETURN_SUCCESS("Updated the projector location");
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
