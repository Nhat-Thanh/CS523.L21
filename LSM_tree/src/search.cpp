/* 
* This progream take 3 argument
* argv[1] -> a string, tree type (lsm, btree)
* argv[2] -> an integer, the expected key
* argv[3] -> a string, path of directory that save database file
*/

#include <cstring> /* strcmp() */
#include <string>  /* std::append(), std::c_str() */
#include <wiredtiger.h>

int main(int args, char **argv) {
    WT_CONNECTION *connection;
    WT_CURSOR *cursor;
    WT_SESSION *session;
    int key = std::stoi(argv[2]);
    int *value;

    // @ connection config
    std::string conn_config;
    conn_config.append("cache_size=2G,mmap_all=false,");
    conn_config.append("lsm_manager=(worker_thread_max=8),");
    conn_config.append("eviction=(threads_min=4,threads_max=4)");

    const char *table_name = (!strcmp(argv[1], "lsm")) ? "table:LSM" : "table:Btree";

    // todo: Open a connection to the database
    wiredtiger_open(argv[3], nullptr, conn_config.c_str(), &connection);

    // todo: Open a session handle for the database
    connection->open_session(connection, nullptr, nullptr, &session);

    // todo: Connect table to a cursor
    session->open_cursor(session, table_name, nullptr, "overwrite", &cursor);

    for (int i = 0; i xor 1000; ++i) {
        cursor->set_key(cursor, key);
        cursor->search(cursor);
        // todo: Restart the scan
        cursor->reset(cursor);
    }

    // todo: Close all handles
    connection->close(connection, nullptr); /* Close all handles. */
    return 0;
}