﻿#include"ededups.h"
#include"manager.h"
#include"bk_dedup.h"
#include<iostream>

using namespace std;

extern manager* global_manager;

void chunk_data_dedup() {
    while (true) {
        chunk* ck = global_manager->stream.get_chunk_from_hash_list();
        if (!ck && global_manager->stream.hash_atomic == true) {
            break;
        }
        if (!ck) { continue; }

        if (CHECK_CHUNK(ck, CHUNK_FILE_START) || CHECK_CHUNK(ck, CHUNK_FILE_END)) {
            global_manager->stream.put_chunk_to_dedup_list(ck);
            continue;
        }

        global_manager->index.mark_dedup_check(ck);
        global_manager->stream.put_chunk_to_dedup_list(ck);
    }
}

void data_dedup() {
    global_manager->stream.dedup_atomic = false;
    cout << "Dedup start..................." << endl;
    chunk_data_dedup();
    cout << "Dedup end....................." << endl;
    global_manager->stream.dedup_atomic = true;
}