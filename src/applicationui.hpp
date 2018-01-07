/*
 * Copyright (c) 2013-2015 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>
#include <QSettings>
#include <QVariantMap>
#include <QVariantList>
#include <bb/cascades/QmlDocument>
#include <qdropbox/QDropbox.hpp>
#include <qdropbox/Account.hpp>
#include "controllers/QDropboxController.hpp"
#include "util/DateUtil.hpp"
#include "util/FileUtil.hpp"
#include <bb/system/SystemToast>
#include <QStringList>
#include <QList>
#include <qdropbox/SharedLink.hpp>
#include <bb/system/InvokeRequest>
#include <QMap>
#include "Logger.hpp"

namespace bb {
    namespace cascades {
        class LocaleHandler;
    }
    namespace system {
        class InvokeManager;
        class InvokeRequest;
    }
}

using namespace bb::cascades;
using namespace bb::system;

class QTranslator;

/*!
 * @brief Application UI object
 *
 * Use this object to create and init app UI, to create context objects, to register the new meta types etc.
 */
class ApplicationUI: public QObject {
    Q_OBJECT
    Q_PROPERTY(bool autoload READ isAutoloadEnabled WRITE setAutoloadEnabled NOTIFY autoloadChanged);
    Q_PROPERTY(QVariantList sharedFiles READ getSharedFiles WRITE setSharedFiles NOTIFY sharedFilesChanged);
public:
    ApplicationUI();
    virtual ~ApplicationUI();

    Q_INVOKABLE QVariant prop(const QString& key, const QVariant& defaultValue = "");
    Q_INVOKABLE void setProp(const QString& key, const QVariant& val);
    Q_INVOKABLE void resendNotification();
    Q_INVOKABLE void authorize();
    Q_INVOKABLE void logout();
    Q_INVOKABLE void toast(const QString& message);
    Q_INVOKABLE QString getRandomColor() const;
    Q_INVOKABLE QString getAccountId() const;
    Q_INVOKABLE bool copyToClipboard(const QString& str);
    Q_INVOKABLE void shareText(const QString& str);
    Q_INVOKABLE QVariantMap getSharedLink(const QString& path);
    Q_INVOKABLE void auth(const QString& token);
    Q_INVOKABLE void invokeFeedback();
    Q_INVOKABLE void clearCache();

    const bool& isAutoloadEnabled() const;
    void setAutoloadEnabled(const bool& autoload);

    const QVariantList& getSharedFiles() const;
    void setSharedFiles(const QVariantList& sharedFiles);

    Q_SIGNALS:
        void currentAccountLoaded(const QVariantMap& accountMap);
        void propChanged(const QString& key, const QVariant& val);
        void sharedLinksLoaded();
        void autoloadChanged(const bool& autoload);
        void sharedFilesChanged(const QVariantList& sharedFiles);

private slots:
    void onSystemLanguageChanged();
    void onAccessTokenObtained(const QString& accessToken);
    void onCurrentAccountLoaded(Account* account);
    void onShared();
    void onSharedLinksLoaded(const QList<SharedLink*>& links);
    void onSharedLinkCreated(SharedLink* link);
    void onInvoke(const bb::system::InvokeRequest& req);
    void onSharedLinkRevoked(const QString& sharedLinkUrl);
    void headlessInvoked();
    void onFeedbackInvoked();
private:
    QSettings m_settings;
    QStringList m_palette;
    QVariantList m_sharedFiles;

    QTranslator* m_translator;
    bb::cascades::LocaleHandler* m_localeHandler;
    bb::system::InvokeManager* m_invokeManager;

    QDropbox* m_pQdropbox;
    Account* m_pAccount;

    QDropboxController* m_pQdropboxController;
    FileUtil* m_pFileUtil;
    DateUtil* m_pDateUtil;

    QString m_downloadsFolder;
    SystemToast m_toast;
    QMap<QString, SharedLink*> m_sharedLinksMap;
    bool m_watchCamera;

    void configureQml();
    void startHeadless();
    void initSignals();
    void clearDir(const QString& path);

    static Logger logger;
};

#endif /* ApplicationUI_HPP_ */
