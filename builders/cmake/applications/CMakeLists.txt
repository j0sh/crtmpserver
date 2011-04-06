cmake_minimum_required(VERSION 2.6)
PROJECT(applications)

SET(CRTMPSERVER_ROOT "${CRTMPSERVER_ROOT}/applications")

ADD_SUBDIRECTORY(flvplayback flvplayback)
ADD_SUBDIRECTORY(appselector appselector)
ADD_SUBDIRECTORY(samplefactory samplefactory)
ADD_SUBDIRECTORY(vptests vptests)
ADD_SUBDIRECTORY(admin admin)
ADD_SUBDIRECTORY(proxypublish proxypublish)
ADD_SUBDIRECTORY(stresstest stresstest)
ADD_SUBDIRECTORY(applestreamingclient applestreamingclient)
ADD_SUBDIRECTORY(vmapp vmapp)

