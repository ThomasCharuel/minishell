#!/bin/bash

shell=$1

instructions=(
    "exit"
    "exit exit"
    "exit hola"
    "exit hola que tal"
    "exit 42"
    "exit 00000042"
    "exit 666"
    "exit 666 666"
    "exit -666 666"
    "exit hola 666"
    "exit 666 666 666 666"
    "exit 666 hola 666"
	"exit hola 666 666"
	"exit 259"
	"exit -4"
	"exit -42"
	"exit -00000042"
	"exit -256"
	"exit -666"
	"exit +666"
	"exit 0"
	"exit +0"
	"exit -0"
	"exit +42"
	"exit -69 -96"
	"exit --666"
	"exit ++++666"
	"exit ++++++0"
	"exit ------0"
	"exit '666'"
	"exit '-666'"
	"exit '+666'"
	"exit '----666'"
	"exit '++++666'"
	"exit '6'66"
	"exit '2'66'32'"
	"exit -'666'666666"
	"exit 9223372036854775807"
	"exit 9223372036854775808"
	"exit -9223372036854775808"
	"exit -9223372036854775809"
)

for instruction in "${instructions[@]}"; do
	if [ "$shell" == "minishell" ]; then
		./minishell "$instruction"
	else
		echo "Running instruction: $instruction"
		(
			$instruction
		)
	fi
	echo "Exit status: $?"
	echo "==============================="
done
