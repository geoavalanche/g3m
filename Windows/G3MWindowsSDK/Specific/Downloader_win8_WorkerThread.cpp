//
//  Storage_win8.hpp
//  G3MWindowsSDK
//
//  Created by F. Pulido on 26/09/14.
//  Copyright (c) 2012 IGO Software SL. All rights reserved.

#include "pch.h"
#include "Downloader_win8_WorkerThread.hpp"


Downloader_win8_WorkerThread::Downloader_win8_WorkerThread(const Downloader_win8* downloader) :
	_downloader(downloader)
{
}


void Downloader_win8_WorkerThread::start(){

	//std::thread first(isStopping());
	//first.
}

void Downloader_win8_WorkerThread::stop(){

}

bool Downloader_win8_WorkerThread::isStopping(){

	return _stopping;
}

void Downloader_win8_WorkerThread::run(){

}

Downloader_win8_WorkerThread::~Downloader_win8_WorkerThread(){
}
