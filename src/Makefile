MOSQUITTOSRC=../../mosquitto/src

CFLAGS=-fPIC -I$(MOSQUITTOSRC) -Wall -Werror -fomit-frame-pointer -funroll-loops -DDEBUG

all: nordbe-auth.so

nordbe-auth.so : plugin.c mysql.o bcrypt.a
	$(CC) ${CFLAGS} -fPIC -shared $^ -o $@ 

mysql.o: mysql.c
	$(CC) ${CFLAGS} -fPIC -shared $^ -o $@ 

bcrypt.a: bcrypt.o crypt_blowfish/crypt_blowfish.a
	ar r bcrypt.a bcrypt.o crypt_blowfish/crypt_blowfish.a

bcrypt.o: bcrypt.c
	$(CC) $(CFLAGS) -c bcrypt.c

crypt_blowfish/crypt_blowfish.a:
	$(MAKE) -C crypt_blowfish crypt_blowfish.a

clean :
	rm *.out
	rm -f *.o *.so 
	rm -f *.o bcrypt_test bcrypt.a *~ core
	$(MAKE) -C crypt_blowfish clean
