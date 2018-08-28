/*********************************************************************************
**
** Copyright (c) 2017 The University of Notre Dame
** Copyright (c) 2017 The Regents of the University of California
**
** Redistribution and use in source and binary forms, with or without modification,
** are permitted provided that the following conditions are met:
**
** 1. Redistributions of source code must retain the above copyright notice, this
** list of conditions and the following disclaimer.
**
** 2. Redistributions in binary form must reproduce the above copyright notice, this
** list of conditions and the following disclaimer in the documentation and/or other
** materials provided with the distribution.
**
** 3. Neither the name of the copyright holder nor the names of its contributors may
** be used to endorse or promote products derived from this software without specific
** prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
** EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
** OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
** SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
** INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
** TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
** BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
** CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
** IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
** SUCH DAMAGE.
**
***********************************************************************************/

// Contributors:
// Written by Peter Sempolinski, for the Natural Hazard Modeling Laboratory, director: Ahsan Kareem, at Notre Dame

#include "remotedatainterface.h"

Q_LOGGING_CATEGORY(remoteInterface, "Remote Interface")
Q_LOGGING_CATEGORY(rawHTTP, "Raw HTTP")

RemoteDataInterface::RemoteDataInterface(QObject *parent):QObject(parent) {}

QString RemoteDataInterface::interpretRequestState(RequestState theState)
{
    switch (theState)
    {
    case RequestState::GOOD:
        return "Request Successful";
    case RequestState::PENDING:
        return "Request In Progress";
    case RequestState::INTERNAL_ERROR:
        return "Internal error creating reply object";
    case RequestState::INVALID_STATE:
        return "Network interface method invoked in invalid state";
    case RequestState::UNKNOWN_TASK:
        return "Task for reply is not recognized";
    case RequestState::SIGNAL_OBJ_MISMATCH:
        return "Network reply does not match reply object";
    case RequestState::SERVICE_UNAVAILABLE:
        return "Remote job service is unavailable";
    case RequestState::LOST_INTERNET:
        return "Lost Internet connection. Please check connection and restart program.";
    case RequestState::DROPPED_CONNECTION:
        return "Remote service has dropped connection.";
    case RequestState::NO_CHANGE_DIR:
        return "Change Directory Failed";
    case RequestState::FILE_NOT_FOUND:
        return "File Not found";
    case RequestState::JOB_SYSTEM_DOWN:
        return "Remote job system may be down for maintainance";
    case RequestState::BAD_HTTP_REQUEST:
        return "Invalid HTTP request";
    case RequestState::GENERIC_NETWORK_ERROR:
        return "Network error in remote request";
    case RequestState::REMOTE_SERVER_ERROR:
        return "Remote server has internal error";
    case RequestState::NOT_IMPLEMENTED:
        return "Feature Not Implemented";
    case RequestState::LOCAL_FILE_ERROR:
        return "Unable to open local file";
    case RequestState::JSON_PARSE_ERROR:
        return "JSON parse failed";
    case RequestState::MISSING_REPLY_STATUS:
        return "Missing status string in task reply";
    case RequestState::MISSING_REPLY_DATA:
        return "Expected data from remote reply missing or mal-formed";
    case RequestState::EXPLICIT_ERROR:
        return "Remote system unable to complete request";
    case RequestState::INVALID_PARAM:
        return "Parameters given for task are invalid.";
    case RequestState::NOT_READY:
        return "Interface is not ready to enact task";
    case RequestState::UNCLASSIFIED:
        return "An unclassified error occured";
    case RequestState::STOPPED_BY_USER:
        return "Task stopped by user";
    }
    return "INTERNAL ERROR";
}

QString RemoteDataInterface::removeDoubleSlashes(QString stringIn)
{
    QString ret;

    for (int i = 0; i < stringIn.size(); i++)
    {
        if (stringIn[i] != '/')
        {
            ret.append(stringIn[i]);
        }
        else if ((i == stringIn.length() - 1) || (stringIn[i + 1] != '/'))
        {
            ret.append(stringIn[i]);
        }
    }
    return ret;
}

RemoteDataReply::RemoteDataReply(QObject * parent):QObject(parent) {}
