# exemplo de makefile com variaveis
# variaveis
INCLUDES = 
NAME = ex02
SOURCES = ${NAME}.c 
OBJFILES = ${NAME}.o
EXEC = ${NAME}

# Mensagem recebida por parametros
MSG = ${msg}

# regras de sufixo
.SUFFIXES : .c .o
.SUFFIXES : .s .o

${OBJFILES}: ${SOURCES} ${INCLUDES}

# como transformar um .c num .o ; $< -- nome do ficheiro
.c.o:
	gcc -ansi -Wall -g -c $<
	
# como transformar um .c num .o ; $< -- nome do ficheiro
.s.o:
	gcc -Wall -g -c $<
	
all: ${EXEC}

${EXEC}: ${OBJFILES}
	gcc -Wall -g -o ${EXEC} ${OBJFILES}

run: ${EXEC}
	./${EXEC}

clean:
	rm -f ${OBJFILES} ${EXEC}
	
# Configuração do GIT
gitconfig:
	git config --global user.email "1151052@isep.ipp.pt"
	git config --global user.name "BrunoPatrao"
	git config --global push.default simple
	
# Update do GIT
git:
	git add -A
	git commit -a -m "${MSG}"
	git pull
	git push
	
#debug
debug:
	ddd ${EXE} &
