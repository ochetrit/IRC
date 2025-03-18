/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nclassea <nclassea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:57:43 by ochetrit          #+#    #+#             */
/*   Updated: 2025/03/18 16:53:11 by nclassea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"

// Constructeurs
Channel::Channel()
	: _topic(""), _invite_only(false), _channel_key(false),
	  _password(""), _user_limit(false), _max_user(FD_MAX) {
	memset(_client_indexs, false, FD_MAX);
	memset(_fd_operator, false, FD_MAX);
}

Channel::Channel(std::string topic, bool inviteOnly, bool channelKey, std::string password, bool userLimit, unsigned int maxUser)
	: _topic(topic), _invite_only(inviteOnly), _channel_key(channelKey),
	  _password(password), _user_limit(userLimit), _max_user(maxUser) {
	memset(_client_indexs, false, FD_MAX);
	memset(_fd_operator, false, FD_MAX);
}

// Getters
std::string Channel::getTopic() const { return _topic; }
bool Channel::isInviteOnly() const { return _invite_only; }
bool Channel::hasChannelKey() const { return _channel_key; }
std::string Channel::getChanPass() const { return _password; }
bool Channel::hasUserLimit() const { return _user_limit; }
unsigned int Channel::getMaxUsers() const { return _max_user; }
bool Channel::isClientInChannel(unsigned int client) const { return _client_indexs[client]; }
bool Channel::isOperator(unsigned int client) const { return _fd_operator[client]; }

// Setters
void Channel::setTopic(const std::string &topic) { _topic = topic; }
void Channel::setInviteOnly(bool inviteOnly) { _invite_only = inviteOnly; }
void Channel::setChannelKey(bool channelKey) { _channel_key = channelKey; }
void Channel::setChanPass(const std::string &password) { _password = password; }
void Channel::setUserLimit(bool userLimit) { _user_limit = userLimit; }
void Channel::setMaxUsers(unsigned int maxUser) { _max_user = maxUser; }
void Channel::addClient(unsigned int client) { _client_indexs[client] = true; }
void Channel::removeClient(unsigned int client) { _client_indexs[client] = false; }
void Channel::setOperator(unsigned int client, bool isOp) { _fd_operator[client] = isOp; }

// void	IRC::send_message(std::string &channel, std::string &message, unsigned int client)
// {
// 	std::string prefix = ":" + _clients[client]._nickname + "!" + _clients[client]._username + "@localhost ";
// 	prefix += message;
// 	print(RED << prefix << RESET);
// 	Map::iterator cha = _channels.find(channel);
// 	if (cha == _channels.end()) {
// 		print(RED << "Erreur: Channel " << channel << " introuvable !" << RESET);
// 		return;
// 	}
// 	for (unsigned int i = 0; i < FD_MAX; i++)
// 	{
// 		if (cha->second._client_indexs[i] && i != client)
// 		{
// 			print(GREEN << "J envoie un message au fd: " << _fds[i].fd << RESET);
// 			ssize_t bytes_sent = send(_fds[i].fd, message.c_str(), message.size(), 0);
// 			if (bytes_sent == -1) {
// 				perror("send");
// 				print(RED << "Erreur lors de l'envoi du message à fd " << _fds[i].fd << RESET);}
// 		}
// 	}
// }