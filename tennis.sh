#!/bin/bash

playerOneScore=50
playerTwoScore=50
ball_area=0
game_over=false


init_game(){
	echo " Player 1: ${playerOneScore}         Player 2: ${playerTwoScore}"
	echo " --------------------------------- "
	echo " |       |       #       |       | "
	echo " |       |       #       |       | "
	echo " |       |       #       |       | "
	echo " |       |       #       |       | "
	echo " |       |       O       |       | "
	echo " |       |       #       |       | "
	echo " |       |       #       |       | "
	echo " |       |       #       |       | "
	echo " --------------------------------- "
}

print_board(){
	echo " Player 1: ${playerOneScore}         Player 2: ${playerTwoScore}"
	echo " --------------------------------- "
	echo " |       |       #       |       | "
	echo " |       |       #       |       | "
	echo " |       |       #       |       | "
	echo " |       |       #       |       | "
	ball_position
	echo " |       |       #       |       | "
	echo " |       |       #       |       | "
	echo " |       |       #       |       | "
	echo " --------------------------------- "
}

choose1_number(){
	echo "PLAYER 1 PICK A NUMBER: "
	read -s player1_num
	if  [[ $player1_num -gt $playerOneScore  || ! $player1_num =~ ^[0-9]+$ ]]; then
		echo "NOT A VALID MOVE !"
		choose1_number	
	fi
}

choose2_number(){
	echo "PLAYER 2 PICK A NUMBER: "
	read -s player2_num
	if  [[ $player2_num -gt $playerTwoScore  || ! $player2_num =~ ^[0-9]+$ ]]; then
		echo "NOT A VALID MOVE !"
		choose2_number	
	fi
}
update_game(){
	playerOneScore="$((playerOneScore-player1_num))"
	playerTwoScore="$((playerTwoScore-player2_num))"
	print_board
	
	echo -e "       Player 1 played: ${player1_num}\n       Player 2 played: ${player2_num}\n\n"	
}

ball_position(){
	curr=" |       |       O       |       | "
	#if 1 lost
	if [[ $player1_num -lt $player2_num ]]; then
		case $ball_area in
		-2) curr="O|       |       #       |       | "
		echo "$curr"
		ball_area=-3
		;;
		-1) curr=" |   O   |       #       |       | "
		echo "$curr"
		ball_area=-2
		;;
		0) curr=" |       |   O   #       |       | "
		echo "$curr"
		ball_area=-1
		;;
		1) curr=" |       |   O   #       |       | "
		echo "$curr"
		ball_area=-1
		;;
		2) curr=" |       |   O   #       |       | "
		echo "$curr"
		ball_area=-1
		;;
		*)
		esac
	#if 2 lost
	elif [[ $player2_num -lt $player1_num ]]; then
		case $ball_area in
		-2) curr=" |       |       #   O   |       | "
		echo "$curr"
		ball_area=1
		;;
		-1) curr=" |       |       #   O   |       | "
		echo "$curr"
		ball_area=1
		;;
		0)   curr=" |       |       #   O   |       | "
		echo "$curr"
		ball_area=1
		;;
		1) curr=" |       |       #       |   O   | "
		echo "$curr"
		ball_area=2
		;;
		2) curr=" |       |       #       |       |O"
		echo "$curr"
		ball_area=3
		;;
		*)
		esac
	else echo "$curr"
	
	fi
}	

winner(){
	if [[ $ball_area = -3 ]]; then
		echo "PLAYER 2 WINS !"
	
		game_over=true
	
	elif [[ $ball_area = 3 ]];then
		echo "PLAYER 1 WINS !"
		game_over=true
	elif [[ $playerOneScore = 0 && $playerTwoScore -gt 0 ]];then
		echo "PLAYER 2 WINS !"
		game_over=true
	elif [[ $playerTwoScore = 0 && $playerOneScore -gt 0 ]];then
		echo "PLAYER 1 WINS !"
		game_over=true
	elif [[ $playerOneScore = 0 && $playerTwoScore = 0 ]];then
		if [[ $ball_area = 0 ]];then
			echo "IT'S A DRAW !"
			game_over=true
		elif [[ $ball_area -gt 0 ]];then
			echo "PLAYER 1 WINS !"
			game_over=true
		else echo  "PLAYER 2 WINS !"
		game_over=true
		fi
	fi
}

init_game
while ! $game_over
do
	choose1_number
	choose2_number
	update_game
	winner
	
done

