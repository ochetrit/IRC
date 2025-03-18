/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nclassea <nclassea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:24:01 by nclassea          #+#    #+#             */
/*   Updated: 2025/03/18 15:01:01 by nclassea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <iostream>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <map>
#include <sstream>
#include <algorithm>
#include <poll.h>
#include <signal.h>
#include <map>
#include <functional>
#include <netdb.h>
#include <vector>
#include <ctime>

class Client {
	public:
		Client();
		Client(std::string nickname, std::string username, std::string pass, std::string host);
		
		std::string getNickname() const;
		std::string getUsername() const;
		std::string getPass() const;
		std::string getHost() const;
		
		void setNickname(const std::string &nickname);
		void setUsername(const std::string &username);
		void setPass(const std::string &pass);
		void setHost(const std::string &host);

	private:
		std::string _nickname;
		std::string _username;
		std::string _pass;
		std::string _host;
};

#endif