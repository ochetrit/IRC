/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nclassea <nclassea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 13:24:39 by ochetrit          #+#    #+#             */
/*   Updated: 2025/03/18 16:44:19 by nclassea         ###   ########.fr       */
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
#include <signal.h>
#include <map>
#include <functional>
#include <netdb.h>
#include <vector>
#include <ctime>
#include "Channel.hpp"
#include "Client.hpp"

#define print(x) std::cout << x << std::endl
#define err(x) std::cerr << x << std::endl
#define MAX_PORT 65535
#define FD_MAX 1021
// #define Map std::map<std::string, t_channel>
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
	public :
		typedef void (IRC::*CommandFunc)(int clientFd, const std::string &cmd);
	
		IRC(unsigned int port, std::string password);
		IRC(const IRC &other);
	
		~IRC();
	
		IRC	&operator=(const IRC &other);
	
		unsigned int	getPort();
		unsigned int	getNbclients();
		struct pollfd	*getFds();
		Client		&getClient(unsigned int client);
		int				getClientfd(unsigned int client);
		std::map<std::string, Channel> &getChannel();
		// std::string		getPassword();
		// std::string		getNick(unsigned int client);
		// std::string		getUser(unsigned int client);
		// std::string		getHost(unsigned int client);
	
		void	add_fds(int fd);
		void	set_client_empty(unsigned int client);
		void	set_client_nickname(unsigned int client, std::string &buffer);
		void	set_client_username(unsigned int client, std::string &buffer);
		void	set_client_pass(unsigned int client, std::string &buffer);
		void	set_client_host(unsigned int client, std::string &buffer);
		void	decremente_nbclient();
		bool	compare_nickname(std::string nickname);
		bool	compare_username(std::string username);
		//void	setServeven
	
		// main func
		int init_server_socket();
		// std::string getClientHost(int client_fd);
		void handle_new_client(int server_fd);
		void handle_client_command(int client_index, const std::string &command);
		void handle_client_data(int client_index);
		void poll_clients(int server_fd);
		void remove_client(unsigned int client_index);
		void	add_channel(std::string name, unsigned int client);
		void	add_client_channel(std::string name, unsigned int client);
		void	send_message(std::string &channel, std::string &message, unsigned int client);
		void	clearCommands();
		void	clearClientBuffers();
	
		// UTILS
		std::string get_prefix(int clientFd);
		void sendAndDisplay(int client_fd, std::string msg);
		void init_cmds();
		void cleanup();
		bool isValidNickname(const std::string &nickname);
		bool isNicknameTaken(const std::string &nickname);
		
	private :
		unsigned int _port;
		std::string _password;
		struct pollfd _fds[FD_MAX];
		// std::map<int, Client> _clients; SUREMENT UTILISER MAP PR LES ITERATORS A VOIR AVEC OSCAR
		std::map<int, Client> _clients;
		std::map<std::string, Channel> _channels;
		unsigned int	_nb_clients;
		std::string _servername;
		// Map	_channels;
		std::map<int, std::string> _client_buffers;

		
		std::map<std::string, CommandFunc> _commands;
		void passCmd(int client_index, const std::string &command);
		void nickCmd(int client_index, const std::string &command);
		void userCmd(int client_index, const std::string &command);
		void pingCmd(int client_index, const std::string &command);
		void quitCmd(int client_index, const std::string &command);
		void privmsg(int client_index, const std::string &command);
		void joinCmd(int client_index, const std::string &command);
		IRC();
	
};



#endif