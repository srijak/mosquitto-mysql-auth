MOSQUITTOSRC=-I../../mosquitto/src
MYSQLINC=-I/usr/include/mysql   -lmysqlclient  
CFLAGS=-fPIC $(MOSQUITTOSRC) -Wall -Werror -fomit-frame-pointer -funroll-loops -DDEBUG

all: nordbe-auth.so

nordbe-auth.so : plugin.c mysql.c bcrypt.a  
	$(CC) ${CFLAGS} -fPIC -shared $^  -L. -o $@   $(MYSQLINC)  

bcrypt.a: bcrypt.o crypt_blowfish/crypt_blowfish.a
	ar r bcrypt.a bcrypt.o crypt_blowfish/*.o

bcrypt.o: bcrypt.c
	$(CC) $(CFLAGS) -c bcrypt.c

crypt_blowfish/crypt_blowfish.a:
	$(MAKE) -C crypt_blowfish crypt_blowfish.a

clean :
	rm -f *.out
	rm -f *.a
	rm -f *.o *.so 
	rm -f *.o bcrypt_test bcrypt.a *~ core
	$(MAKE) -C crypt_blowfish clean
