/**
 * People API
 * An API that returns a list of people based on a provided date-time.
 *
 * The version of the OpenAPI document: 1.0.0
 *
 * NOTE: This class is auto generated by OpenAPI Generator (https://openapi-generator.tech).
 * https://openapi-generator.tech
 * Do not edit the class manually.
 */

#ifndef ER__ER_DefaultApi_H
#define ER__ER_DefaultApi_H

#include "ER_Helpers.h"
#include "ER_HttpRequest.h"
#include "ER_ServerConfiguration.h"
#include "ER_Oauth.h"

#include "ER__people_get_200_response_inner.h"
#include <QString>

#include <QObject>
#include <QByteArray>
#include <QStringList>
#include <QList>
#include <QNetworkAccessManager>

namespace er {

class ER_DefaultApi : public QObject {
    Q_OBJECT

public:
    ER_DefaultApi(const int timeOut = 0);
    ~ER_DefaultApi();

    void initializeServerConfigs();
    int setDefaultServerValue(int serverIndex,const QString &operation, const QString &variable,const QString &val);
    void setServerIndex(const QString &operation, int serverIndex);
    void setApiKey(const QString &apiKeyName, const QString &apiKey);
    void setBearerToken(const QString &token);
    void setUsername(const QString &username);
    void setPassword(const QString &password);
    void setTimeOut(const int timeOut);
    void setWorkingDirectory(const QString &path);
    void setNetworkAccessManager(QNetworkAccessManager* manager);
    int addServerConfiguration(const QString &operation, const QUrl &url, const QString &description = "", const QMap<QString, ER_ServerVariable> &variables = QMap<QString, ER_ServerVariable>());
    void setNewServerForAllOperations(const QUrl &url, const QString &description = "", const QMap<QString, ER_ServerVariable> &variables =  QMap<QString, ER_ServerVariable>());
    void setNewServer(const QString &operation, const QUrl &url, const QString &description = "", const QMap<QString, ER_ServerVariable> &variables =  QMap<QString, ER_ServerVariable>());
    void addHeaders(const QString &key, const QString &value);
    void enableRequestCompression();
    void enableResponseCompression();
    void abortRequests();
    QString getParamStylePrefix(const QString &style);
    QString getParamStyleSuffix(const QString &style);
    QString getParamStyleDelimiter(const QString &style, const QString &name, bool isExplode);

    /**
    * @param[in]  datetime QDateTime [required]
    */
    void peopleGet(const QDateTime &datetime);


private:
    QMap<QString,int> _serverIndices;
    QMap<QString,QList<ER_ServerConfiguration>> _serverConfigs;
    QMap<QString, QString> _apiKeys;
    QString _bearerToken;
    QString _username;
    QString _password;
    int _timeOut;
    QString _workingDirectory;
    QNetworkAccessManager* _manager;
    QMap<QString, QString> _defaultHeaders;
    bool _isResponseCompressionEnabled;
    bool _isRequestCompressionEnabled;
    ER_HttpRequestInput _latestInput;
    ER_HttpRequestWorker *_latestWorker;
    QStringList _latestScope;
    OauthCode _authFlow;
    OauthImplicit _implicitFlow;
    OauthCredentials _credentialFlow;
    OauthPassword _passwordFlow;
    int _OauthMethod = 0;

    void peopleGetCallback(ER_HttpRequestWorker *worker);

signals:

    void peopleGetSignal(QList<ER__people_get_200_response_inner> summary);

    void peopleGetSignalFull(ER_HttpRequestWorker *worker, QList<ER__people_get_200_response_inner> summary);

    Q_DECL_DEPRECATED_X("Use peopleGetSignalError() instead")
    void peopleGetSignalE(QList<ER__people_get_200_response_inner> summary, QNetworkReply::NetworkError error_type, QString error_str);
    void peopleGetSignalError(QList<ER__people_get_200_response_inner> summary, QNetworkReply::NetworkError error_type, const QString &error_str);

    Q_DECL_DEPRECATED_X("Use peopleGetSignalErrorFull() instead")
    void peopleGetSignalEFull(ER_HttpRequestWorker *worker, QNetworkReply::NetworkError error_type, QString error_str);
    void peopleGetSignalErrorFull(ER_HttpRequestWorker *worker, QNetworkReply::NetworkError error_type, const QString &error_str);

    void abortRequestsSignal();
    void allPendingRequestsCompleted();

public slots:
    void tokenAvailable();
};

} // namespace er
#endif