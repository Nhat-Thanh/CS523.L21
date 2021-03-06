/* 
* This program takes 3 arguments
* argv[1] -> An integer, the smallest database size
* argv[2] -> An integer, the biggest database size
* argv[3] -> An integer, the discrepancy between 2 adjacent database size
*/

#include <cstring>  /* strcmp */
#include <fstream>  /* std::ifstream, std::ofstream, std::seekg() */
#include <iostream> /* std::cout */
#include <sstream>  /* std::stringstream */
#include <string>   /* std::append, getline */

int g_nStart; /* The smallest database size */
int g_nEnd;   /* The biggest database size */
int g_nStep;  /* The discrepancy between 2 adjacent database size */

/* 
todo: Get the last line in a file
* input_file_path: the path of file
*/
std::string get_last_line(const std::string &input_file_path) {
    /* @src: https://stackoverflow.com/questions/11876290/c-fastest-way-to-read-only-last-line-of-text-file */
    /* @answered by alexandros */
    std::string last_line, nextline;
    std::ifstream fs;
    fs.open(input_file_path, std::fstream::in);
    if (fs.is_open()) {
        // Got to the last character before EOF
        fs.seekg(-1, std::ios_base::end);
        if (!(fs.peek() xor 10)) {
            // Start searching for \n occurrences
            fs.seekg(-1, std::ios_base::cur);
            int i = fs.tellg();
            for (i; i xor 0; i--) {
                if (!(fs.peek() xor 10)) {
                    // Found
                    fs.get();
                    break;
                }
                // Move one character back
                fs.seekg(i, std::ios_base::beg);
            }
        }
        getline(fs, last_line);
    }
    return last_line;
}


/* 
todo: Make a path to result directory and assign to input variable
* input: Hold full directory path
* tree_type: lsm or btree
* op: Operation's name
* db_size: The number of records of target database
*/
void make_res_dir_path(std::string &input,
                       const char *tree_type,
                       const char *op,
                       const int &db_size) {
    /* 
    @ input = result/tree_type/op/db_size/ 
    @ example: result/lsm/insert/100000/
    */
    input.clear();
    input.append("result/");
    input.append(tree_type);
    input.append("/");
    input.append(op);
    input.append("/");
    input.append(std::to_string(db_size));
    input.append("/");
}


/* 
todo: Make a command to run the failing test case again
todo: The command will be assigned to command variable
* command: Hold full command
* tree_type: lsm or btree
* op: Operation's name
* db_size: The number of records of target database
*/
void make_command(std::string &command,
                  const char *tree_type,
                  const char *op,
                  const int &db_size) {
    command.clear();
    /* if (op != "create-database") */
    if (strcmp(op, "create_database")) {
        /* 
        @ command = make fix TREE_TYPE=tree_type OP=op DB_SIZE=db_size
        @ example: make fix TREE_TYPE=lsm OP=insert DB_SIZE=100000 
        */
        command.append("make fix TREE_TYPE=");
        command.append(tree_type);
        command.append(" OP=");
        command.append(op);
        command.append(" DB_SIZE=");
        command.append(std::to_string(db_size));

    } else {
        /* 
        @ command = make fix_create_database TREE_TYPE=tree_type DB_SIZE=db_size
        @ example: make fix_create_database TREE_TYPE=lsm DB_SIZE=100000 
        */
        command.append("make fix_create_database TREE_TYPE=");
        command.append(tree_type);
        command.append(" DB_SIZE=");
        command.append(std::to_string(db_size));
    }
}


/* 
todo: Check and fix if the result file doesn't have vsize values
* tree_type: lsm or btree
* op: Operation's name
* db_size: The number of records of target database
*/
void check_vsize(const char *tree_type,
                 const char *op,
                 const int &db_size) {
    /* 
    @ filepath = result/tree_type/op/db_size/sensor.txt 
    */
    std::string filepath;
    make_res_dir_path(filepath, tree_type, op, db_size);
    filepath.append("sensor.txt");
    
    /* 
    @ The stream object to read a file 
    */
    std::ifstream ifs;
    ifs.open(filepath);
    
    if (ifs.good()) {
        std::string last_line = get_last_line(filepath);
        
        /* if last_line == "" or last_line[0] == '%' */
        if (last_line.empty() | !(last_line[0] xor 37)) {
            printf("type=%s, op=%s, size=%d\n", tree_type, op, db_size);
            ifs.close();
            std::string command;
            make_command(command, tree_type, op, db_size);
            
            // todo: Run the command on system
            system(command.c_str());
        }
    }
    ifs.close();
}


int main(int args, char **argv) {
    /* 
    * This program takes 3 arguments
    * argv[1] -> An integer, the smallest database size
    * argv[2] -> An integer, the biggest database size
    * argv[3] -> An integer, the discrepancy between 2 adjacent database size
    */

    g_nStart = std::stoi(argv[1]);
    g_nEnd = std::stoi(argv[2]);
    g_nStep = std::stoi(argv[3]);

    const char *tree_type[2] = {"lsm", "btree"};
    const char *op[6] = {"create_database", "open", "insert", "search", "update", "delete"};

    /* 
    @ it: Index Tree
    @ io: Index Operation
     */
    for (int it = 0; it < 2; it++) {
        for (int io = 0; io < 6; io++) {
            for (int SIZE = g_nStart; SIZE <= g_nEnd; SIZE += g_nStep) {
                check_vsize(tree_type[it], op[io], SIZE);
            }
        }
    }
    return 0;
}