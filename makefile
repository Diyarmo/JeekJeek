CC=g++
STD=-std=c++14 -Wall -pedantic
CF=$(STD)
BUILD_DIR=build
   
all: $(BUILD_DIR) server.out

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/response.o: utils/response.cpp utils/response.hpp utils/include.hpp
	$(CC) $(CF) -c utils/response.cpp -o $(BUILD_DIR)/response.o

$(BUILD_DIR)/request.o: utils/request.cpp utils/request.hpp utils/include.hpp utils/utilities.hpp
	$(CC) $(CF) -c utils/request.cpp -o $(BUILD_DIR)/request.o

$(BUILD_DIR)/utilities.o: utils/utilities.cpp utils/utilities.hpp
	$(CC) $(CF) -c utils/utilities.cpp -o $(BUILD_DIR)/utilities.o

$(BUILD_DIR)/server.o: server/server.cpp server/server.hpp server/route.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp
	$(CC) $(CF) -c server/server.cpp -o $(BUILD_DIR)/server.o

$(BUILD_DIR)/route.o: server/route.cpp server/route.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp
	$(CC) $(CF) -c server/route.cpp -o $(BUILD_DIR)/route.o

$(BUILD_DIR)/server_main.o: server/main.cpp server/server.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp
	$(CC) $(CF) -c server/main.cpp -o $(BUILD_DIR)/server_main.o

$(BUILD_DIR)/jeekjeek.o: jeekjeek/jeekjeek.cpp $(BUILD_DIR)/database.o
	$(CC) $(CF) -c jeekjeek/jeekjeek.cpp -o $(BUILD_DIR)/jeekjeek.o 

$(BUILD_DIR)/database.o: jeekjeek/database.cpp $(BUILD_DIR)/content.o
	$(CC) $(CF) -c jeekjeek/database.cpp -o $(BUILD_DIR)/database.o 

$(BUILD_DIR)/content.o: jeekjeek/content.cpp $(BUILD_DIR)/user.o
	$(CC) $(CF) -c jeekjeek/content.cpp -o $(BUILD_DIR)/content.o 

$(BUILD_DIR)/user.o: jeekjeek/user.cpp
	$(CC) $(CF) -c jeekjeek/user.cpp -o $(BUILD_DIR)/user.o 

$(BUILD_DIR)/jeekjeekF.o: $(BUILD_DIR)/user.o $(BUILD_DIR)/content.o $(BUILD_DIR)/database.o $(BUILD_DIR)/jeekjeek.o
	$(CC) $(CF) -c $(BUILD_DIR)/user.o $(BUILD_DIR)/content.o $(BUILD_DIR)/database.o $(BUILD_DIR)/jeekjeek.o -o $(BUILD_DIR)/jeekjeekF.o

server.out: 	$(BUILD_DIR)/response.o $(BUILD_DIR)/request.o $(BUILD_DIR)/utilities.o $(BUILD_DIR)/server.o $(BUILD_DIR)/route.o $(BUILD_DIR)/server_main.o $(BUILD_DIR)/jeekjeek.o $(BUILD_DIR)/database.o $(BUILD_DIR)/content.o $(BUILD_DIR)/user.o
	$(CC) $(CF) $(BUILD_DIR)/response.o $(BUILD_DIR)/request.o $(BUILD_DIR)/utilities.o $(BUILD_DIR)/server.o $(BUILD_DIR)/route.o $(BUILD_DIR)/server_main.o $(BUILD_DIR)/jeekjeek.o $(BUILD_DIR)/database.o $(BUILD_DIR)/content.o $(BUILD_DIR)/user.o -o server.out

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) *.o *.out &> /dev/null

