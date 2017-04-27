/*
 * AwaBridge.hpp
 *
 *  Created on: Jan 12, 2017
 *      Author: Zarnowski
 */

#ifndef CI40_PROJECT5_SRC_AWABRIDGE_HPP_
#define CI40_PROJECT5_SRC_AWABRIDGE_HPP_

#include "awa/ServerSession.hpp"
#include "spdlog/spdlog.h"
#include "GenericSensor.hpp"
#include <memory>
#include <vector>
#include <string>


class AwaBridge {

	public:
		AwaBridge(std::string address, int port, std::shared_ptr<spdlog::logger> l);
		virtual ~AwaBridge();

		void establishSessionIfNeeded();
		void releaseSession();

		void subscribeIfNeeded(const std::vector<std::shared_ptr<Sensor>>&);
		void unsubscribe(const std::vector<std::shared_ptr<Sensor>>&);

		void process();
		std::shared_ptr<awa::ServerSession> getSession() const;
		const std::string& getAddress() const;
		const int& getPort() const;
	private:
		std::string address;
		int port;
		std::shared_ptr<spdlog::logger> logger;

		std::shared_ptr<awa::ServerSession> session;

		void allocAndConnect();
};

#endif /* CI40_PROJECT5_SRC_AWABRIDGE_HPP_ */
