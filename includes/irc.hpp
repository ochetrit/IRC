/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nclassea <nclassea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 13:24:39 by ochetrit          #+#    #+#             */
/*   Updated: 2025/02/24 16:40:34 by nclassea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP

#define IRC_HPP

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
#include <map>
#include "clients.hpp"

#define print(x) std::cout << x << std::endl
#define err(x) std::cerr << x << std::endl
#define MAX_PORT 65535
#define FD_MAX 1021
#define Map std::map<std::string, t_channel>
#define MAG "\e[0;35m"
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define PURPLE "\033[35m"


// irssi USE THIS CLIENT

class IRC
{
	private :

	unsigned int _port;
	std::string _password;
	struct pollfd _fds[FD_MAX];
	t_clients	_clients[FD_MAX];
	unsigned int	_nb_clients;
	Map	_channels;


	IRC();

	public :

	IRC(unsigned int port, std::string password);
	IRC(const IRC &other);

	~IRC();

	IRC	&operator=(const IRC &other);

	unsigned int	getPort();
	unsigned int	getNbclients();
	struct pollfd	*getFds();
	t_clients		&getClient(unsigned int client);
	int				getClientfd(unsigned int client);
	std::string		getPassword();
	Map				&getChannel();

	void	add_fds(int fd);
	void	set_client_empty(unsigned int client);
	void	set_client_nickname(unsigned int client, std::string &buffer);
	void	set_client_username(unsigned int client, std::string &buffer);
	void	set_client_pass(unsigned int client, std::string &buffer);
	void	decremente_nbclient();
	bool	compare_nickname(std::string nickname);
	bool	compare_username(std::string username);
	//void	setServeven

	// main func
	int		init_server_socket(IRC &irc);
	void	handle_new_client(IRC &irc, int server_fd);
	void	handle_client_command(IRC &irc, int client_index, const std::string &command);
	void	handle_client_data(IRC &irc, int client_index);
	void	poll_clients(IRC &irc, int server_fd);
	void	remove_client(unsigned int client_index);
	void	add_channel(std::string name, unsigned int client);
	void	add_client_channel(std::string name, unsigned int client);
	void	send_message(std::string &channel, std::string &message, unsigned int client);

	
};



#endif