/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nclassea <nclassea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 13:33:57 by nclassea          #+#    #+#             */
/*   Updated: 2025/03/18 15:01:04 by nclassea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

Client::Client() : _nickname(""), _username(""), _pass(""), _host("") {}

Client::Client(std::string nickname, std::string username, std::string pass, std::string host)
	: _nickname(nickname), _username(username), _pass(pass), _host(host) {}

std::string Client::getNickname() const { return _nickname; }
std::string Client::getUsername() const { return _username; }
std::string Client::getPass() const { return _pass; }
std::string Client::getHost() const { return _host; }

void Client::setNickname(const std::string &nickname) { _nickname = nickname; }
void Client::setUsername(const std::string &username) { _username = username; }
void Client::setPass(const std::string &pass) { _pass = pass; }
void Client::setHost(const std::string &host) { _host = host; }
