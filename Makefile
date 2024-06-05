# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cy4gate_mmarinelli <cy4gate_mmarinelli@    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/02 15:02:31 by mmarinel          #+#    #+#              #
#    Updated: 2024/06/05 22:00:45 by cy4gate_mma      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			:= npuzzle


all: $(NAME)

.configure:
	@cmake -DCMAKE_BUILD_TYPE=Release -B build .

build: .configure
	@make --silent -C build

run: build
	@reset && ./bin/$(NAME) &

$(NAME): build

clean:
	@rm -rf build

fclean: clean
	@rm -rf ./bin/npuzzle

re: fclean all

.PHONY: all clean fclean re .configure 
