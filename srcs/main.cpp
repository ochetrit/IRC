/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nino <nino@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 13:23:06 by ochetrit          #+#    #+#             */
/*   Updated: 2025/03/05 17:08:17 by nino             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/irc.hpp"

std::string IRC::get_prefix(int clientFd) {
	return (":" + getNick(clientFd) + "!" + getUser(clientFd) + "@" + getHost(clientFd));
}

bool	check_args(char *av)
{
	std::string port(av);

	if (port.find_first_not_of("0123456789") != std::string::npos)
	{
		err("I only need numbers for my port");
		return false;
	}

	else if (port.size() > 5)
	{
		err("Port has to be < " << MAX_PORT);
		return false;
	}

	return true;
}

bool check_arguments(int ac, char **av) {
	if (ac != 3) {
		err("I need 2 arguments in the format: <port> <password>");
		return false;
	}
	if (!check_args(av[1]))
		return false;
	return true;
}

void IRC::sendAndDisplay(int client_fd, std::string msg) {
	std::cout << BLUE << "[client: " << client_fd << "]: " << RESET << msg;
	send(getFds()[client_fd].fd, msg.c_str(), msg.length(), 0);
}

std::string getServerCreationDate() {
	std::time_t now = std::time(NULL);
	std::tm* tm = std::localtime(&now);

	char buffer[80];
	std::strftime(buffer, sizeof(buffer), "%a %b %d %Y %H:%M:%S UTC", tm);
	return std::string(buffer);
}

int IRC::init_server_socket() {
	int server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server_fd == -1) {
		perror("Socket failed");
		return -1;
	}

	int option = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0) {
		perror("setsockopt failed");
		close(server_fd);
		return -1;
	}

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
	server_addr.sin_port = htons(getPort());

	if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		perror("Error bind");
		close(server_fd);
		return -1;
	}

	if (listen(server_fd, FD_MAX) < 0) {
		perror("Error listen");
		close(server_fd);
		return -1;
	}

	return server_fd;
}

std::string getClientHost(int client_fd) {
	struct sockaddr_in addr;
	socklen_t addr_len = sizeof(addr);

	if (getpeername(client_fd, (struct sockaddr*)&addr, &addr_len) == -1) {
		perror("getpeername failed");
		return "unknown";
	}

	char ip_str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &addr.sin_addr, ip_str, sizeof(ip_str));

	struct hostent *host = gethostbyaddr(&addr.sin_addr, sizeof(addr.sin_addr), AF_INET);
	if (host)
		return std::string(host->h_name);

	return std::string(ip_str);
}

void IRC::handle_new_client(int server_fd) {
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);

	if (client_fd < 0) {
		perror("Error accept");
		return;
	}
	
	std::string client_host = getClientHost(client_fd);
	print(GREEN << "Client connecté depuis : " << client_host << RESET);
	print(PURPLE << client_fd << RESET);
	add_fds(client_fd);
	set_client_empty(getNbclients());
}

void IRC::init_cmds() {
	_commands["PASS"] = &IRC::passCmd;
	_commands["NICK"] = &IRC::nickCmd;
	_commands["USER"] = &IRC::userCmd;
	// _commands["MODE"] = &IRC::mode;
	_commands["JOIN"] = &IRC::joinCmd;
	// _commands["TOPIC"] = &IRC::topicCmd;
	_commands["PRIVMSG"] = &IRC::privmsg;
	_commands["QUIT"] = &IRC::quitCmd;
	_commands["PING"] = &IRC::pingCmd;
	// _commands["KICK"] = &IRC::kick;
	// _commands["INVITE"] = &IRC::invite;
}

void IRC::passCmd(int client_index, const std::string &command) {
	std::string clientPass = command.substr(5);
	set_client_pass(client_index, clientPass);
	if (getClient(client_index)._pass != getPassword()) {
		print(RED << "Connection with client " << client_index << " failed: wrong password " << RESET);
		std::string errMsg = "ERROR :Wrong password\r\n";
		// std::string errMsg = ": 464 " + getNick(client_index) + " Wrong password\r\n";
		send(getFds()[client_index].fd, errMsg.c_str(), errMsg.size(), 0);
		// sendAndDisplay(client_index, errMsg);
		close(getFds()[client_index].fd);
		getFds()[client_index] = getFds()[getNbclients() - 1];
		decremente_nbclient();
	}
	else
		print(GREEN << "New client accepted" << RESET);
}

bool IRC::isValidNickname(const std::string &nickname) {
	if (nickname.length() > 9 || nickname.empty() || !isalpha(nickname[0]))
		return false;
	for (size_t i = 1; i < nickname.length(); ++i) {
		if (!isalnum(nickname[i]) && nickname[i] != '-' && nickname[i] != '_' && nickname[i] != '|')
			return false;
	}
	return true;
}

bool IRC::isNicknameTaken(const std::string &nickname) {
	for (int i = 0; i < FD_MAX; i++) {
		if (_clients[i]._nickname == nickname)
			return true;
	}
	return false;
}


void IRC::nickCmd(int client_index, const std::string &command) {
	std::string nickname = command.substr(5);

	if (!isValidNickname(nickname)) {
		sendAndDisplay(client_index, ':' + _servername + " 432 " + nickname + " :Not a valid nickname\r\n");
		return;
	}

	if (isNicknameTaken(nickname)) {
		sendAndDisplay(client_index, ':' + _servername + " 433 " + nickname + " :Nickname is already in use\r\n");
		return;
	}
	
	while (!compare_nickname(nickname)) {
		std::ostringstream oss;
		oss << nickname << "[" << client_index << "]";
		nickname = oss.str();
	}
	std::string old_nick = getNick(client_index);
	set_client_nickname(client_index, nickname);
	std::cout << "nickname : " << nickname << std::endl;
}

void IRC::userCmd(int client_index, const std::string &command) {
	std::istringstream iss(command);
	std::string cmd, username, hostname, servername, realname;
	std::string creationDate = getServerCreationDate();

	if (_clients[client_index]._pass.empty()) {
		sendAndDisplay(client_index, ":myserver.com 464 * :Password required\r\n");
		return;
	}
	
	iss >> cmd >> username >> hostname >> servername;
	std::getline(iss, realname);

	if (username.empty() || hostname.empty() || servername.empty() || realname.empty()) {
		sendAndDisplay(client_index, ":" + _servername + " 461 " + getNick(client_index) + " :Missing arguments\r\n");
		return;
	}

	if (realname[1] != ':') {
		sendAndDisplay(client_index, ":" + _servername + " 461 " + getNick(client_index) + " :Missing ':' before realname\r\n");
	}

	realname.erase(0, realname.find_first_not_of(' '));
	realname.erase(realname.find_last_not_of(' ') + 1);

	// Afficher les informations parsées
	print(GREEN << "Parsed USER command - Username: " << username 
				<< ", Hostname: " << hostname
				<< ", Servername: " << servername
				<< ", Realname: " << realname << RESET);

	while (!compare_username(username)) {
		std::ostringstream oss;
		oss << username << "[" << client_index << "]";
		username = oss.str();
	}

	set_client_username(client_index, username);
	set_client_host(client_index, hostname);

	std::string welcome = ":server 001 " + getNick(client_index) + " :Welcome to the IRC Network, " + getNick(client_index) + "!" + username + '@' + hostname + "\r\n"; 
	std::string servInfo = ":server 002 " + getNick(client_index) + " :Your host is " + _servername + " ! \r\n";
	std::string currentInfo = ":server 003 " + getNick(client_index) + " :This server was created " + creationDate + "\r\n";
	sendAndDisplay(client_index, welcome);
	sendAndDisplay(client_index, servInfo);
	sendAndDisplay(client_index, currentInfo);

}

void IRC::pingCmd(int client_index, const std::string &command) {
	std::string pongResponse = "PONG :" + command.substr(5) + "\r\n";
	sendAndDisplay(client_index, pongResponse);
}

void IRC::quitCmd(int client_index, const std::string &command) {
	std::string quit = command.substr(6);
	print(PURPLE << getClient(client_index)._nickname << " is deconnected because: "<< quit << RESET);
}

void IRC::joinCmd(int client_index, const std::string &command) {
	std::string name = command.substr(6);
	print(RED << (getChannel().find(name) != getChannel().end()) << RESET);
	if (getChannel().find(name) != getChannel().end())
		add_client_channel(name, client_index);
	else
		add_channel(name, client_index);
}

void IRC::privmsg(int client_index, const std::string &command) {
	size_t startChannel = command.find("#") + 1;
	size_t endChannel = command.find(" ", startChannel);
	std::string channel = command.substr(startChannel, endChannel - startChannel);
	std::string message = command;
	send_message(channel, message, client_index);
}


void IRC::remove_client(unsigned int client_index) {
	int fd_to_close = getFds()[client_index].fd;
	
	close(fd_to_close);

	if (client_index != getNbclients() - 1) {
		getFds()[client_index] = getFds()[getNbclients() - 1];
		_clients[client_index] = _clients[getNbclients() - 1];
	}

	decremente_nbclient();
	
	//print(RED << "Client " << client_index << " déconnecté." << RESET);
}

void IRC::handle_client_command(int client_index, const std::string &command) {
	std::string cmd = command.substr(0, command.find(' '));

	std::map<std::string, CommandFunc>::iterator it = _commands.find(cmd);
	if (it != _commands.end()) {
		CommandFunc func = it->second;
		(this->*func)(client_index, command);
	}
	else
		print(RED << "Commande inconnue " << command << RESET);
}

void IRC::handle_client_data(int client_index) {
	char buffer[512] = {0};
	int bytes_received = recv(getClientfd(client_index), buffer, sizeof(buffer) - 1, 0);

	if (bytes_received <= 0) {
		remove_client(client_index);
		return;
	}
	
	_client_buffers[client_index] += std::string(buffer, bytes_received);

	size_t pos;

	while ((pos = _client_buffers[client_index].find('\n')) != std::string::npos) {
		std::string cmd = _client_buffers[client_index].substr(0, pos);
		_client_buffers[client_index].erase(0, pos + 1); // erase cmd 

		cmd.erase(std::remove(cmd.begin(), cmd.end(), '\r'), cmd.end());
		print(YELLOW << "Commande reçue: [" << cmd << "]" << RESET);
		handle_client_command(client_index, cmd);
	}
}

void IRC::poll_clients(int server_fd) {
	print(YELLOW << "En attente des clients..." << RESET);
	
	while (true) {
		int ret = poll(getFds(), getNbclients(), 0);
		if (ret < 0) {
			perror("Erreur poll");
			break;
		}

		// Connexion d'un nouveau client
		if (getFds()[0].revents & POLLIN)
			handle_new_client(server_fd);

		// Données des clients existants
		for (unsigned int i = 1; i < getNbclients(); i++) {
			if (getFds()[i].revents & POLLIN)
				handle_client_data(i);
		}
	}
}

IRC* g_irc = NULL;

void	sign_handler(int signum) {
	print(RED << "\nCaught Ctrl + C (signal " << signum << "), cleaning up..." << RESET);
	if (g_irc) {
		print("here");
		g_irc->clearCommands();
		g_irc->clearClientBuffers();
		for (unsigned int i = 0; i < g_irc->getNbclients(); i++)
			close(g_irc->getFds()[i].fd);
	}
	exit(0);
}

int main(int ac, char **av) {
	if (!check_arguments(ac, av))
		return 1;
		
	IRC irc(atoi(av[1]), av[2]);
	g_irc = &irc;
	irc.init_cmds();

	signal(SIGINT, sign_handler);
	
	int server_fd = irc.init_server_socket();
	if (server_fd < 0)
		return 2;

	irc.add_fds(server_fd);  // Ajoute le serveur au poll
	irc.poll_clients(server_fd);


	close(server_fd);
	return 0;
}

