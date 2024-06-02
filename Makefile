# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmarinel <mmarinel@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/02 15:02:31 by mmarinel          #+#    #+#              #
#    Updated: 2024/06/02 15:38:34 by mmarinel         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			:= npuzzle


all: $(NAME)

debug:
	@$(MAKE) --silent .build BUILD_FLAGS="-DCMAKE_BUILD_TYPE=Debug"

solver:
	@$(MAKE) --silent .build BUILD_FLAGS="-DCMAKE_BUILD_TYPE=Release"

.build:
	@cmake $(BUILD_FLAGS) -B build .
	@make --silent -C build

run:
	@reset && ./bin/$(NAME) &
	@disown

$(NAME): solver run

clean:
	@rm -rf build

fclean: clean
	@rm -rf ./bin/npuzzle

re: fclean all

.PHONY: all clean fclean re .build 
