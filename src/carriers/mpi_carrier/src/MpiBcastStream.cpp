// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Author: Daniel Krieg krieg@fias.uni-frankfurt.de
 * Copyright (C) 2010 Daniel Krieg
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 *
 */

#ifdef CREATE_MPI_CARRIER

#include <yarp/os/impl/MpiBcastStream.h>

using namespace yarp::os::impl;


void MpiBcastStream::startJoin() {
    comm->sema.wait();
    int cmd = -1;
    MPI_Bcast(&cmd, 1, MPI_INT, 0,comm->comm);
}




/////////////////////////////////////////////////
// InputStream

int MpiBcastStream::read(const Bytes& b) {
    if (terminate) {
	return -1;
    }
    if (readAvail == 0) {
        // get new data
        reset();
        int size;
	        #ifdef MPI_DEBUG
                printf("[MpiBcastStream @ %s] Trying to read\n", name.c_str());
                #endif
        MPI_Bcast(&size, 1, MPI_INT, 0,comm->comm);
	#ifdef MPI_DEBUG
                printf("[MpiBcastStream @ %s] got size %d\n", name.c_str(), size);
                #endif
        if (size < 0) {
            // Commands
            if (size == -1) {
		// Connect:
		// Let a new port join the broadcast group
                comm->accept();
            }
            else if (size == -2) {
		// Disconnect:
		// Let a port leave the broadcast group
                #ifdef MPI_DEBUG
                printf("[MpiBcastStream @ %s] Got disconnect\n", name.c_str());
                #endif
                int length;
                MPI_Bcast(&length, 1, MPI_INT, 0,comm->comm);
                char* remote = new char[length];
                MPI_Bcast(remote, length, MPI_CHAR, 0,comm->comm);
                #ifdef MPI_DEBUG
                printf("[MpiBcastStream @ %s] Got disconnect : %s\n", name.c_str(), remote);
                #endif
                terminate = !strcmp(remote, name.c_str());
                #ifdef MPI_DEBUG
                printf("[MpiBcastStream @ %s] should disconnect : %d\n", name.c_str(), terminate);
                #endif
                delete [] remote;
                comm->disconnect(terminate);
            }
            return 0;
        }
        if (size == b.length()) {
            // size of received data matches expected data
            // do not use buffer, but write directly
            MPI_Bcast(b.get(), size, MPI_BYTE, 0, comm->comm);
            return size;
        }
        else {
            // allocate new buffer
            readBuffer = new char[size];
            MPI_Bcast(readBuffer, size, MPI_BYTE, 0, comm->comm);
            //printf("got new msg of size %d\n", size);
            readAvail = size;
            readAt = 0;
        }
    }
    if (readAvail>0) {
        // copy data from buffer to destination object
        int take = readAvail;
        if (take>b.length()) {
            take = b.length();
        }
        memcpy(b.get(),readBuffer+readAt,take);
        //printf("read %d of %d \n", take, readAvail);
        readAt += take;
        readAvail -= take;
        return take;
    }
    return 0;
}

/////////////////////////////////////////////////
// OutputStream

void MpiBcastStream::write(const Bytes& b) {
    	#ifdef MPI_DEBUG
                printf("[MpiBcastStream @ %s] getting sema for write\n", name.c_str());
                #endif
    comm->sema.wait();
    	#ifdef MPI_DEBUG
                printf("[MpiBcastStream @ %s] trying to write\n", name.c_str());
                #endif
    int size = b.length();
    MPI_Bcast(&size, 1, MPI_INT, 0, comm->comm );
    MPI_Bcast(b.get(), size, MPI_BYTE, 0, comm->comm );
    comm->sema.post();
        	#ifdef MPI_DEBUG
                printf("[MpiBcastStream @ %s] done writing\n", name.c_str());
                #endif
}


#endif