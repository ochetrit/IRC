/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nclassea <nclassea@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 13:23:06 by ochetrit          #+#    #+#             */
/*   Updated: 2025/02/24 16:40:24 by nclassea         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/irc.hpp"

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

int IRC::init_server_socket(IRC &irc) {
	int server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server_fd == -1) {
		perror("Socket failed");
		return -1;
	}

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
	server_addr.sin_port = htons(irc.getPort());

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

void IRC::handle_new_client(IRC &irc, int server_fd) {
	struct sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);

	if (client_fd < 0) {
		perror("Error accept");
		return;
	}

	irc.add_fds(client_fd);
	irc.set_client_empty(irc.getNbclients());
	print(GREEN << "Nouveau client accepté" << RESET);
}

void IRC::handle_client_command(IRC &irc, int client_index, const std::string &command) {
	if (command.compare(0, 4, "CAP ") == 0 && command.find("LS") != std::string::npos) {
		print("ignore CAP LS");
	} else if (command.find("PASS ") == 0) {
		std::string clientPass = command.substr(5);
		irc.set_client_pass(client_index, clientPass);
		if (irc.getClient(client_index)._pass != irc.getPassword()) {
			print(RED << "Mot de passe incorrect pour client " << client_index << RESET);
			std::string errMsg = "ERROR :Mot de passe incorrect\r\n";
			send(irc.getFds()[client_index].fd, errMsg.c_str(), errMsg.size(), 0);
			close(irc.getFds()[client_index].fd);
			irc.getFds()[client_index] = irc.getFds()[irc.getNbclients() - 1];
			irc.decremente_nbclient();
		}
	} else if (command.find("NICK ") == 0) {
		std::string nickname = command.substr(5);
		irc.set_client_nickname(client_index, nickname);
	} else if (command.find("USER ") == 0) {
		std::string username = command.substr(5, command.find(' ', 5) - 5);
		irc.set_client_username(client_index, username);
		std::string welcome = ":server 001 " + irc.getClient(client_index)._nickname +
			" :" + irc.getClient(client_index)._nickname + " [" + username + "@localhost] Bienvenue !\r\n";
		send(irc.getFds()[client_index].fd, welcome.c_str(), welcome.size(), 0);
	} else if (command.find("PING ") == 0) {
		std::string pongResponse = "PONG :" + command.substr(5) + "\r\n";
		send(irc.getFds()[client_index].fd, pongResponse.c_str(), pongResponse.size(), 0);
	} else {
		print(RED << "Commande inconnue: " << command << RESET);
	}
}

void IRC::remove_client(unsigned int client_index) {
	int fd_to_close = getFds()[client_index].fd;

	close(fd_to_close);

	if (client_index != getNbclients() - 1) {
		getFds()[client_index] = getFds()[getNbclients() - 1];
		_clients[client_index] = _clients[getNbclients() - 1];
	}

	decremente_nbclient();

	print(RED << "Client " << client_index << " déconnecté." << RESET);
}


void IRC::handle_client_data(IRC &irc, int client_index) {
	char buffer[512] = {0};
	int bytes_received = recv(irc.getClientfd(client_index), buffer, sizeof(buffer) - 1, 0);

	if (bytes_received <= 0) {
		irc.remove_client(client_index);
		return;
	}
	

	std::string tmp(buffer);
	tmp.erase(std::remove(tmp.begin(), tmp.end(), '\r'), tmp.end());

	std::istringstream iss(tmp);
	std::string command;
	while (std::getline(iss, command)) {
		if (!command.empty()) {
			print(YELLOW << "Commande reçue: [" << command << "]" << RESET);
			handle_client_command(irc, client_index, command);
		}
	}
}

void IRC::poll_clients(IRC &irc, int server_fd) {
	print(YELLOW << "En attente des clients..." << RESET);
	while (true) {
		int ret = poll(irc.getFds(), irc.getNbclients(), 0);
		if (ret < 0) {
			perror("Erreur poll");
			break;
		}

		// Connexion d'un nouveau client
		if (irc.getFds()[0].revents & POLLIN)
			handle_new_client(irc, server_fd);

		// Données des clients existants
		for (unsigned int i = 1; i < irc.getNbclients(); i++) {
			if (irc.getFds()[i].revents & POLLIN)
				handle_client_data(irc, i);
		}
	}
}

int main(int ac, char **av) {
	if (!check_arguments(ac, av))
		return 1;

	IRC irc(atoi(av[1]), av[2]);
	int server_fd = irc.init_server_socket(irc);
	if (server_fd < 0)
		return 2;

	irc.add_fds(server_fd);  // Ajoute le serveur au poll
	irc.poll_clients(irc, server_fd);

	close(server_fd);
	return 0;
}





// int	main(int ac, char **av)
// {
// 	if (ac != 3)
// 	{
// 		err("I need 2 arguments in the format: <port> <password>");
// 		return 1;
// 	}

// 	if (!check_args(av[1]))
// 		return 2;

// 	IRC	irc(atoi(av[1]), av[2]);

// 	int server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

// 	if (server_fd == -1)
// 	{
// 		perror("Socket failed");
// 		return 3;
// 	}

// 	struct sockaddr_in server_addr;
// 	memset(&server_addr, 0, sizeof(server_addr));
// 	server_addr.sin_family = AF_INET;
// 	inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
// 	server_addr.sin_port = htons(irc.getPort());

// 	if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
// 	{
// 		perror("Error bind");
// 		close(server_fd);
// 		return 4;
// 	}

// 	irc.add_fds(server_fd);

// 	print(YELLOW << "Waiting for clients...");

// 	if (listen(server_fd, FD_MAX) < 0)
// 	{
// 		perror("Error listen");
// 		close(server_fd);
// 		return 1;
// 	}


// 	while (true)
// 	{
// 		int ret = poll(irc.getFds(), irc.getNbclients(), 0); /// Pas sur de l utilisation de poll demander a chatgpt
// 		if (ret < 0) {
// 			perror("Poll error");
// 			break;
// 		}
// 		if (irc.getFds()[0].revents & POLLIN) // Check de nv client
// 		{
// 			struct sockaddr_in client_addr;
// 			socklen_t client_len = sizeof(client_addr);
// 			int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
// 			if (client_fd < 0)
// 			{
// 				perror("Error accept");
// 				close(server_fd);
// 				return 1;
// 			}
// 			// print(GREEN << "New client accepted");
// 			irc.add_fds(client_fd);
// 			irc.set_client_empty(irc.getNbclients());
// 			// std::string str = "Set a nickname\n"; // pas sur de devoir l afficher vu que irssi nous envoie deja le username et le nickname
// 			// send(client_fd, str.c_str(), str.size(), 0);
// 		}

// 		for (unsigned int i = 1; i < irc.getNbclients(); i++) // Se balader parmi tous les clients
// 		{
// 			if (irc.getFds()[i].revents & POLLIN)
// 			{
// 				char buffer[512];
// 				memset(buffer, 0, sizeof(buffer));

// 				int bytes_received = recv(irc.getClientfd(i), buffer, sizeof(buffer) - 1, 0);

// 				if (bytes_received <= 0)
// 				{
// 					print(RED << "Client " << i << " deconnecte" << RESET);
// 					close(irc.getFds()[i].fd);
// 					irc.getFds()[i] = irc.getFds()[irc.getNbclients() - 1];
// 					irc.decremente_nbclient();
// 					i--;
// 					continue;
// 				}

// 				buffer[bytes_received] = '\0';

// 				std::string tmp(buffer);

// 				// nettoyer les retours \r
// 				tmp.erase(std::remove(tmp.begin(), tmp.end(), '\r'), tmp.end());
				

// 				// decoupe par sauts de lignes les commandes
// 				std::istringstream iss(tmp);
// 				std::string command;

// 				while (std::getline(iss, command)) {
// 					if (command.empty())
// 						continue;
			
// 					print(YELLOW << "Commande reçue: [" << command << "]" << RESET);

// 					if (command.compare(0, 4, "CAP ") == 0 && command.find("LS") != std::string::npos) {
// 						print("ignore CAP LS");
// 						// Il faut en fait l ignorer 
// 						// std::string response = ":server CAP * LS :\r\n";
// 						// send(irc.getFds()[i].fd, response.c_str(), response.size(), 0);
// 						// print(PURPLE << "Réponse CAP envoyée" << RESET);
// 					}

// 					else if (command.find("PASS ") == 0) {
// 						std::string clientPass = command.substr(5);
// 						irc.set_client_pass(i, clientPass);
// 						print("Client " << i << " - Pass: " << irc.getClient(i)._pass);
// 						if (irc.getClient(i)._pass != irc.getPassword()) {
// 							print(RED << "Client " << i << " a fourni un mauvais mot de passe. Déconnexion." << RESET);
// 							std::string errMsg = "ERROR :Mot de passe incorrect\r\n";
// 							send(irc.getFds()[i].fd, errMsg.c_str(), errMsg.size(), 0);
// 							close(irc.getFds()[i].fd);
// 							irc.getFds()[i] = irc.getFds()[irc.getNbclients() - 1]; // Remplacement par le dernier fd
// 							irc.decremente_nbclient();
// 							i--;
// 							break;
// 						} else {
// 							print(GREEN << "Client " << i << " a fourni le bon mot de passe." << RESET);
// 						}
// 					}
					
// 					else if (command.find("NICK ") == 0) {
// 						std::string nickname = command.substr(5);
// 						irc.set_client_nickname(i, nickname);
// 						print("Client " << i << " - Nickname: " << irc.getClient(i)._nickname);

// 					}
// 					else if (command.find("USER ") == 0) {
// 						std::string username = command.substr(5, command.find(' ', 5) - 5);
// 						irc.set_client_username(i, username);
// 						print("Client " << i << " - Username: " << irc.getClient(i)._username);

// 						// Optionnel: Envoie un message de bienvenue
// 						std::string nickname = irc.getClient(i)._nickname;
// 						std::string welcome = ":server 001 " + nickname + " :" + nickname + " [" + username + "@localhost] Bienvenue sur le serveur IRC!\r\n";
// 						send(irc.getFds()[i].fd, welcome.c_str(), welcome.size(), 0);
// 						print(GREEN << "Message de bienvenue envoyé : " << welcome << RESET);
// 					}
// 					else if (command.find("PING ") == 0)
// 					{
// 						std::string pingContent = command.substr(5);
// 						std::string pongResponse = "PONG :" + pingContent + "\r\n";
// 						send(irc.getFds()[i].fd, pongResponse.c_str(), pongResponse.size(), 0);
// 						print(GREEN << "Répondu au PING avec: " << pongResponse << RESET);
// 					}
// 					else {
// 						print(RED << "Commande non reconnue: " << command << RESET);
// 					}
// 				}
// 			}
// 		}
// 	}
// 	//close(client_fd);m
// 	close(server_fd);

// 	return 0;
// }
