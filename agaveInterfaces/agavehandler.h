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

#ifndef AGAVEHANDLER_H
#define AGAVEHANDLER_H

#include "../remotedatainterface.h"

#include <QNetworkReply>
#include <QHttpMultiPart>
#include <QFile>

#include <QJsonDocument>
#include <QJsonObject>

enum class AgaveRequestType {AGAVE_GET, AGAVE_POST, AGAVE_DELETE, AGAVE_UPLOAD, AGAVE_PIPE_UPLOAD, AGAVE_PIPE_DOWNLOAD, AGAVE_DOWNLOAD, AGAVE_PUT, AGAVE_NONE, AGAVE_APP};

class AgaveTaskGuide;
class AgaveTaskReply;
//class AgaveLongRunning;

class AgaveHandler : public RemoteDataInterface
{
    Q_OBJECT

    friend class AgaveTaskReply;

public:
    explicit AgaveHandler();
    ~AgaveHandler();

    virtual QString getUserName();
    virtual bool isDisconnected();

    virtual RemoteDataReply * closeAllConnections();

    //Defaults to directory root,
    //Subsequent commands with remote folder names are either absolute paths
    //or reletive to the current working directory
    virtual RemoteDataReply * setCurrentRemoteWorkingDirectory(QString cd);

    //Remote tasks to be implemented in subclasses:
    //Returns a RemoteDataReply, which should have the correct signal attached to an appropriate slot
    virtual RemoteDataReply * performAuth(QString uname, QString passwd);

    virtual RemoteDataReply * remoteLS(QString dirPath);

    virtual RemoteDataReply * deleteFile(QString toDelete);
    virtual RemoteDataReply * moveFile(QString from, QString to);
    virtual RemoteDataReply * copyFile(QString from, QString to);
    virtual RemoteDataReply * renameFile(QString fullName, QString newName);

    virtual RemoteDataReply * mkRemoteDir(QString location, QString newName);

    virtual RemoteDataReply * uploadFile(QString location, QString localFileName);
    virtual RemoteDataReply * uploadBuffer(QString location, QByteArray fileData, QString newFileName);
    virtual RemoteDataReply * downloadFile(QString localDest, QString remoteName);
    virtual RemoteDataReply * downloadBuffer(QString remoteName);

    virtual RemoteDataReply * runRemoteJob(QString jobName, ParamMap jobParameters, QString remoteWorkingDir, QString indivJobName = "");

    virtual RemoteDataReply * getListOfJobs();
    virtual RemoteDataReply * getJobDetails(QString IDstr);
    virtual RemoteDataReply * stopJob(QString IDstr);

    //-----------------------------------------
    //Agave Specific Functions:

    QString getTenantURL();
    bool inShutdownMode();

public slots:
    //On Agave Apps:
    //Register info on the Agave App's parameters, using:
    void registerAgaveAppInfo(QString agaveAppName, QString fullAgaveName, QStringList parameterList, QStringList inputList, QString workingDirParameter);
    //After that, use the standard runRemoteJob, where jobName is the agaveAppName,
    //the job parameters are a list matching the inputs/parameters given by parameterList and inputList
    //and the remoteWorkingDir will be used as a input/parameter named in remoteDirParameter (optional)

    //For debugging purposes, to retrive the list of available Agave Apps:
    AgaveTaskReply * getAgaveAppList();

    void sendCounterPing(QString urlForPing);
    RemoteDataReply * runAgaveJob(QJsonDocument rawJobJSON);

signals:
    void finishedAllTasks();

protected:
    void handleInternalTask(AgaveTaskReply *agaveReply, QNetworkReply * rawReply);

private slots:
    void finishedOneTask();

private:
    AgaveTaskReply * performAgaveQuery(QString queryName);
    AgaveTaskReply * performAgaveQuery(QString queryName, QMap<QString, QByteArray> varList, QObject *parentReq = nullptr);

    QNetworkReply * distillRequestData(AgaveTaskGuide * theGuide, QMap<QString, QByteArray> * varList);
    QNetworkReply * finalizeAgaveRequest(AgaveTaskGuide * theGuide, QString urlAppend, QByteArray * authHeader = nullptr, QByteArray postData = "", QIODevice * fileHandle = nullptr);

    void forwardReplyToParent(AgaveTaskReply * agaveReply, RequestState replyState);
    void forwardReplyToParent(AgaveTaskReply * agaveReply, RequestState replyState, QString param1);

    void clearAllAuthTokens();

    void setupTaskGuideList();
    void insertAgaveTaskGuide(AgaveTaskGuide * newGuide);
    AgaveTaskGuide * retriveTaskGuide(QString taskID);

    QString getPathReletiveToCWD(QString inputPath);

    static QString removeDoubleSlashes(QString stringIn);

    QNetworkAccessManager networkHandle;
    QSslConfiguration SSLoptions;
    const QString tenantURL = "https://agave.designsafe-ci.org";
    const QString clientName = "SimCenter_CWE_GUI";
    const QString storageNode = "designsafe.storage.default";

    QByteArray authEncoded;
    QByteArray clientEncoded;
    QByteArray token;
    QByteArray tokenHeader;
    QByteArray refreshToken;

    QString authUname;
    QString authPass;
    QString clientKey;
    QString clientSecret;

    QMap<QString, AgaveTaskGuide*> validTaskList;

    QString pwd = "";

    int pendingRequestCount = 0;
    bool performingShutdown = false;
    bool authGained = false;
    bool attemptingAuth = false;
};

#endif // AGAVEHANDLER_H
