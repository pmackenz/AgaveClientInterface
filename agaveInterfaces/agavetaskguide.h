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

#ifndef AGAVETASKGUIDE_H
#define AGAVETASKGUIDE_H

#include <QStringList>

enum class AgaveRequestType;

enum class AuthHeaderType {NONE, PASSWD, CLIENT, TOKEN, REFRESH};

//TODO: This whole class, needs more documentation in particular
class AgaveTaskGuide
{
public:
    explicit AgaveTaskGuide();
    explicit AgaveTaskGuide(QString newID, AgaveRequestType reqType);

    void setURLsuffix(QString newValue);
    void setHeaderType(AuthHeaderType newValue);

    void setTokenFormat(bool newSetting);
    void setDynamicURLParams(QString format);
    void setDynamicURLParams(QString format, QList<QString> subNames);
    void setPostParams(QString format);
    void setPostParams(QString format, QList<QString> subNames);
    void setAsInternal();

    void setAgaveFullName(QString newFullName);
    void setAgavePWDparam(QString newPWDparam);
    void setAgaveParamList(QStringList newParamList);
    void setAgaveInputList(QStringList newInputList);

    QString getTaskID();
    QByteArray getURLsuffix();
    QByteArray getArgAndURLsuffix(QMap<QString, QByteArray> * varList = nullptr);
    AgaveRequestType getRequestType();
    AuthHeaderType getHeaderType();
    QByteArray fillPostArgList(QMap<QString, QByteArray> * argList = nullptr);
    QByteArray fillURLArgList(QMap<QString, QByteArray> * argList = nullptr);
    bool isTokenFormat();
    bool isInternal();

    QString getAgaveFullName();
    QString getAgavePWDparam();
    QStringList getAgaveParamList();
    QStringList getAgaveInputList();

    bool usesPostParms();
    bool usesURLParams();

private:
    QString taskId;

    QString URLsuffix = "";
    AgaveRequestType requestType;
    AuthHeaderType headerType = AuthHeaderType::NONE;

    QByteArray fillAnyArgList(QMap<QString, QByteArray> * argList, QList<QString> * subNames, QString * strFormat);

    bool internalTask = false;
    bool usesTokenFormat = false;

    QString postFormat = "";
    QString dynURLFormat = "";
    QStringList postVarNames;
    QStringList urlVarNames;

    QString agaveFullName;
    QString agavePWDparam;
    QStringList agaveParamList;
    QStringList agaveInputList;
};

#endif // AGAVETASKGUIDE_H
