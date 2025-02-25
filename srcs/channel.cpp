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
	channels.insert(std::make_pair(name, new_channel));
}


void	IRC::add_client_channel(std::string name, unsigned int client)
{
	channels[name]._client_indexs[client] = true;
}