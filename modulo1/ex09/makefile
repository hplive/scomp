# exemplo de makefile com variaveis
# variaveis
INCLUDES = 
NAME = ex09
SOURCES = ${NAME}.c 
OBJFILES = ${NAME}.o
EXEC = ${NAME}

# Mensagem recebida por parametros
MSG = ${msg}

# Modulo recebido por parametros
MOD=MODULO${mod}

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
	git config --global user.email "hplive@hotmail.com"
	git config --global user.name "helder pereira"
	git config --global push.default simple
	
# Update do GIT
git:
	git add -A
	git commit -a -m "${MSG}"
	git pull
	git push

githp:
	make clean
	git add -A
	git commit -a -m "${MOD} ${NAME}"
	git pull
	git push


#debug:
#	ddd ${EXE} &