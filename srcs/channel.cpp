/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ochetrit <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 14:57:43 by ochetrit          #+#    #+#             */
/*   Updated: 2025/02/25 14:57:44 by ochetrit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/irc.hpp"

void	IRC::add_channel(std::string name, unsigned int client)
{
	t_channel new_channel;

	new_channel._topic = "";
	new_channel._invite_only = false;
	new_channel._channel_key = false;
	new_channel._password = "";
	new_channel._user_limit = false;
	new_channel._max_user = FD_MAX;
	memset(new_channel._client_indexs, false, FD_MAX);
	memset(new_channel._fd_operator, false, FD_MAX);
	new_channel._client_indexs[client] = true;
	new_channel._fd_operator[client] = true;
	_channels.insert(std::make_pair(name, new_channel));
	//print(BLUE << "New channel: " << name);
	//print("Operator: " << _clients[client]._username << RESET);
}


void	IRC::add_client_channel(std::string name, unsigned int client)
{
	_channels[name]._client_indexs[client] = true;
	//print(BLUE << _clients[client]._username << " join " << name << RESET);
}

void	IRC::send_message(std::string &channel, std::string &message, unsigned int client)
{
	std::string prefix = ":" + _clients[client]._nickname + "!" + _clients[client]._username + "@localhost ";
	prefix += message;
	print(RED << prefix << RESET);
	Map::iterator cha = _channels.find(channel);
	if (cha == _channels.end()) {
		print(RED << "Erreur: Channel " << channel << " introuvable !" << RESET);
		return;
	}
	for (unsigned int i = 0; i < FD_MAX; i++)
	{
		if (cha->second._client_indexs[i] && i != client)
		{
			print(GREEN << "J envoie un message au fd: " << _fds[i].fd << RESET);
			ssize_t bytes_sent = send(_fds[i].fd, message.c_str(), message.size(), 0);
			if (bytes_sent == -1) {
				perror("send");
				print(RED << "Erreur lors de l'envoi du message à fd " << _fds[i].fd << RESET);}
		}
	}
}