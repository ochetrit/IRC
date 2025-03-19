/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nclassea <nclassea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:18:50 by nclassea          #+#    #+#             */
/*   Updated: 2025/03/18 15:48:50 by nclassea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#define FD_MAX 1021

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

class IRC;

class Channel {
	public:
		Channel();
		Channel(std::string topic, bool inviteOnly, bool channelKey, std::string password, bool userLimit, unsigned int maxUser);

		// Getters
		std::string getTopic() const;
		bool isInviteOnly() const;
		bool hasChannelKey() const;
		std::string getChanPass() const;
		bool hasUserLimit() const;
		unsigned int getMaxUsers() const;
		bool isClientInChannel(unsigned int client) const;
		bool isOperator(unsigned int client) const;

		// Setters
		void setTopic(const std::string &topic);
		void setInviteOnly(bool inviteOnly);
		void setChannelKey(bool channelKey);
		void setChanPass(const std::string &password);
		void setUserLimit(bool userLimit);
		void setMaxUsers(unsigned int maxUser);
		void addClient(unsigned int client);
		void removeClient(unsigned int client);
		void setOperator(unsigned int client, bool isOp);

	private:
		std::string _topic;
		bool _invite_only;
		bool _channel_key;
		std::string _password;
		bool _user_limit;
		unsigned int _max_user;
		bool _client_indexs[FD_MAX];
		bool _fd_operator[FD_MAX];
};

#endif
