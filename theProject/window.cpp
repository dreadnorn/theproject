#include <QtWidgets>
#include <QtWebKitWidgets>
#include <QFile>
#include "window.h"
#include <QDateTime>
#include <QDate>
#include <QTime>

Window::Window(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);
    webView->setFixedSize(640,320);
    QFile trafFile(_TRAFFILE);
    qDebug() << trafFile.open(QIODevice::ReadOnly);
    QTextStream trafStream(&trafFile);
    QString trafString =trafStream.readAll();
    trafFile.close();
    if(trafString.contains('0'))
        followTrueUnfollowFalse = false;
    else
        followTrueUnfollowFalse = true;
    mPaginationNextUrl = "https://api.instagram.com/v1/users/self/follows?access_token=" + _MYTOKEN;
    mPaginationNextUrl.append("&count=" + _UNFCOUNT);
    mTimeCounterUrl = "http://free.timeanddate.com/countdown/i502ug17/n19/cf11/cm0/cu4/ct1/cs1/ca0/co0/cr0/ss0/cac000/cpc000/pct/tcfff/fs100/szw320/szh135/iso";//2015-12-30T10:22:00"
    QFile trapFile(_TRAPFILE);
    qDebug() << trapFile.open(QIODevice::ReadOnly);
    QTextStream trapStream(&trapFile);
    QString trapString = trapStream.readAll();
    trapFile.close();
    _PASSWORD = trapString;
    connect(webView, SIGNAL(loadProgress(int)), this, SLOT(onProgress(int)));
    followTrigerred = false;
    unfollowTrigerred = false;
    likeTrigerred = false;
    webView->settings()->setAttribute(QWebSettings::AutoLoadImages, false);
    this->setWindowState(Qt::WindowMinimized);
    isLoggedIn = false;
    setWindowTitle(_USERNAME);
    mPage = webView->page();
    mPage->networkAccessManager()->setCookieJar(new CookieJar);
    pushButton->setEnabled(false);
    pushButton_2->setEnabled(false);
    pushButton_3->setEnabled(false);
    likeCheckCounter = 30;
    pushButton->setText(" ");
    pushButton_2->setText(" ");
    pushButton_3->setText(" ");
}

void Window::setUrl(const QUrl &url)
{
    webView->setUrl(url);
}

void Window::onProgress(int value)
{
    sdebug << "\t\t\t\t\t" << value;
    progressBar->setValue(value);
    if(value==100)
    {
        progressBar->setValue(0);
        QString plainWeb = webView->page()->mainFrame()->toPlainText();
        QString htmlWeb = webView->page()->mainFrame()->toHtml();

        if(followTrigerred)
        {
            if(!isFollowing(plainWeb)) {
                clickFollowUnFollow();
            }
            QTimer* timer = new QTimer;
            timer->setInterval(5000);
            timer->start();
            followTrigerred = false;
            connect(timer, &QTimer::timeout, [=](){
                webView->page()->history()->clear();
                webView->settings()->clearMemoryCaches();
                renewPage();
                setNextNonPrivateUrl();
                timer->stop();
                timer->deleteLater();
            });

        }
        if(unfollowTrigerred)
        {
            if(isFollowing(plainWeb)) {
                clickFollowUnFollow();
            }
            QTimer* timer = new QTimer;
            timer->setInterval(5000);
            timer->start();
            unfollowTrigerred = false;
            connect(timer, &QTimer::timeout, [=](){
                webView->page()->history()->clear();
                webView->settings()->clearMemoryCaches();
                renewPage();
                setNextNonFollowingUrl();
                timer->stop();
                timer->deleteLater();
            });
        }
        if(likeTrigerred)
        {
            likeTrigerred = false;
            likeClick();
            QTimer* timer = new QTimer;
            timer->setInterval(2000);
            timer->start();
            connect(timer, &QTimer::timeout, [=](){
                timer->stop();
                webView->page()->history()->clear();
                webView->settings()->clearMemoryCaches();
                renewPage();
                if(followTrueUnfollowFalse)
                    obtainUsersToFollow();
                else
                    obtainUsersToUnfollow();
                timer->deleteLater();
            });

        }

        if(!isLoggedIn){
            login();
            isLoggedIn = isLoginSuccessfull(htmlWeb);
            sdebug << isLoggedIn;
        }
    }
}

bool Window::isLoginSuccessfull(QString plainWeb)
{
    if(plainWeb.isEmpty())
        return false;
    if(plainWeb.indexOf("id")!=-1)
    {
        if(followTrueUnfollowFalse)
            obtainUsersToFollow();
        else
            obtainUsersToUnfollow();
        return true;
    }
    else {
        return false;
    }
}
bool Window::isFollowing(QString plainWeb)
{
    if(plainWeb.indexOf("TAKIP ET") != -1){
        return false;
    }
    if(plainWeb.indexOf("TAKIPTESIN") != -1){
        return true;
    }
    return false;
}
void Window::login()
{
    QThread::msleep(500);
    QMouseEvent * event1 = new QMouseEvent ((QEvent::MouseButtonPress), QPoint(270,90), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    qApp->postEvent((QObject*)webView,(QEvent *)event1);
    QMouseEvent * event2 = new QMouseEvent ((QEvent::MouseButtonRelease), QPoint(270,90), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    qApp->postEvent((QObject*)webView,(QEvent *)event2);
    QKeyEvent * event3 = new QKeyEvent((QEvent::KeyPress),Qt::Key_P,Qt::NoModifier, _USERNAME);
    qApp->postEvent((QObject*)webView,(QEvent*)event3);
    QKeyEvent * event4 = new QKeyEvent((QEvent::KeyRelease),Qt::Key_P,Qt::NoModifier,"");
    qApp->postEvent((QObject*)webView,(QEvent*)event4);
    QMouseEvent * event5 = new QMouseEvent ((QEvent::MouseButtonPress), QPoint(270,140), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    qApp->postEvent((QObject*)webView,(QEvent *)event5);
    QMouseEvent * event6 = new QMouseEvent ((QEvent::MouseButtonRelease), QPoint(270,140), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    qApp->postEvent((QObject*)webView,(QEvent *)event6);
    QKeyEvent * event7 = new QKeyEvent((QEvent::KeyPress),Qt::Key_P,Qt::NoModifier, _PASSWORD);
    qApp->postEvent((QObject*)webView,(QEvent*)event7);
    QKeyEvent * event8 = new QKeyEvent((QEvent::KeyRelease),Qt::Key_P,Qt::NoModifier,"");
    qApp->postEvent((QObject*)webView,(QEvent*)event8);
    QMouseEvent * event9 = new QMouseEvent ((QEvent::MouseButtonPress), QPoint(440,190), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    qApp->postEvent((QObject*)webView,(QEvent *)event9);
    QMouseEvent * event10 = new QMouseEvent ((QEvent::MouseButtonRelease), QPoint(440,190), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
    qApp->postEvent((QObject*)webView,(QEvent *)event10);
}
void Window::clickMouse(int x, int y)
{
    QThread::msleep(500);
    qApp->postEvent((QObject*)webView,(QEvent *)new QMouseEvent ((QEvent::MouseButtonPress), QPoint(x,y),
                                                                 Qt::LeftButton,
                                                                 Qt::LeftButton,
                                                                 Qt::NoModifier   ));
    qApp->postEvent((QObject*)webView,(QEvent *)new QMouseEvent ((QEvent::MouseButtonRelease), QPoint(x,y),
                                                                 Qt::LeftButton,
                                                                 Qt::LeftButton,
                                                                 Qt::NoModifier   ));
}
void Window::goToUserUrl(QString userName)
{
    setUrl(QUrl("https://www.instagram.com/" + userName));
}
void Window::followUser(QString userNameToFollow)
{
    goToUserUrl(userNameToFollow);
    followTrigerred = true;
}
void Window::unfollowUser(QString userNameToUnfollow)
{
    goToUserUrl(userNameToUnfollow);
    unfollowTrigerred = true;
}
void Window::likePost(QString url)
{
    likeTrigerred = true;
    setUrl(QUrl(url));
}
void Window::obtainUsersToFollow()
{
    pushButton_2->setText(" ");
    pushButton_3->setText(" ");
    pushButton->setText("Obtaining...");
    QString url = "https://api.instagram.com/v1/users/" + _TARGETUSERID + "/followed-by?access_token=" + _STOLENTOKEN;
    url.append("&count=" + _COUNT);
    QNetworkRequest request = QNetworkRequest(QUrl(url));
    QNetworkAccessManager* tManager = new QNetworkAccessManager;
    QNetworkReply* reply = tManager->get(request);
    connect(reply, &QNetworkReply::finished, [=](){
        globalCounter = 0;
        mUserList.clear();
        mUserIDList.clear();
        mNonPrivateUserList.clear();
        QByteArray tJSON = reply->readAll();
        QJsonDocument document = QJsonDocument::fromJson(tJSON);
        QJsonObject object = document.object();
        int metaCodeInt = object.value("meta").toObject().value("code").toInt();
        sdebug << "Meta Code: " << metaCodeInt;
        if(metaCodeInt == 400)
        {
            sdebug << "meta Error";
            sdebug << tJSON;
            reply->deleteLater();
            tManager->deleteLater();
            return;
        }
        QJsonValue value = object.value("data");
        QJsonArray array = value.toArray();
        foreach(const QJsonValue &v, array){
            mUserList << v.toObject().value("username").toString();
            mUserIDList << v.toObject().value("id").toString();
        }
        reply->deleteLater();
        tManager->deleteLater();
        pushButton->setText("Obtainin finished");
        processNextUser();
    });
}
void Window::obtainUsersToUnfollow()
{
    pushButton_2->setText(" ");
    pushButton_3->setText(" ");
    pushButton->setText("Obtaining...");
    QNetworkRequest request = QNetworkRequest(QUrl(mPaginationNextUrl));
    QNetworkAccessManager* tManager = new QNetworkAccessManager;
    QNetworkReply* reply = tManager->get(request);
    connect(reply, &QNetworkReply::finished, [=](){
        globalCounter = 0;
        mUserList.clear();
        mUserIDList.clear();
        mNonFollowingUserList.clear();
        QByteArray tJSON = reply->readAll();
        QJsonDocument document = QJsonDocument::fromJson(tJSON);
        mPaginationNextUrl = document.object().value("pagination").toObject().value("next_url").toString();
        QJsonObject object = document.object();
        int metaCodeInt = object.value("meta").toObject().value("code").toInt();
        sdebug << "Meta Code: " << metaCodeInt;
        if(metaCodeInt == 400)
        {
            sdebug << "meta Error";
            sdebug << tJSON;
            reply->deleteLater();
            tManager->deleteLater();
            return;
        }
        QJsonValue value = object.value("data");
        QJsonArray array = value.toArray();
        foreach(const QJsonValue &v, array){
            mUserList << v.toObject().value("username").toString();
            mUserIDList << v.toObject().value("id").toString();
        }
        reply->deleteLater();
        tManager->deleteLater();
        pushButton->setText("Obtainin finished");
        processNextUserToUnfollow();
    });
}
void Window::getUserID(QString userName)
{
    QString url = "https://api.instagram.com/v1/users/search?q=" + userName + "&client_id=" + _CLIENTID;
    QNetworkRequest request = QNetworkRequest(QUrl(url));
    QNetworkAccessManager* tManager = new QNetworkAccessManager;
    QNetworkReply* reply = tManager->get(request);
    connect(reply, &QNetworkReply::finished, [=](){
        QByteArray tJSON = reply->readAll();
        QJsonDocument document = QJsonDocument::fromJson(tJSON);
        QJsonObject object = document.object();
        int metaCodeInt = object.value("meta").toObject().value("code").toInt();
        sdebug << "Meta Code" << metaCodeInt;
        if(metaCodeInt == 400)
        {
            sdebug << "meta Error";
            sdebug << tJSON;
            reply->deleteLater();
            tManager->deleteLater();
            return;
        }
        QJsonValue value = object.value("data");
        QJsonArray array = value.toArray();
        value = array.at(0);
        object = value.toObject();
        value = object.value("id");
        emit s_userID(value.toString());
        reply->deleteLater();
        tManager->deleteLater();
    });
}
void Window::getTargetUserID()
{
    getUserID(_TARGETUSER);
    connect(this, &Window::s_userID, [=](QString userID){
        _TARGETUSERID = userID;
    });
}
void Window::processNextUser()
{
    if(globalCounter < mUserList.size())
    {
        pushButton_2->setText("Analizing Users...");
        QString currentUserName = mUserList.at(globalCounter);
        sdebug << "Current User Name" << currentUserName << globalCounter;
        QString userID = mUserIDList.at(globalCounter);
        QString url = "https://api.instagram.com/v1/users/"+ userID + "/relationship?&access_token=" + _STOLENTOKEN;
        QNetworkRequest request = QNetworkRequest(QUrl(url));
        QNetworkAccessManager* tManager = new QNetworkAccessManager;
        QNetworkReply* reply = tManager->get(request);
        connect(reply, &QNetworkReply::finished, [=](){
            QByteArray tJSONins = reply->readAll();
            QJsonDocument documentins = QJsonDocument::fromJson(tJSONins);
            QJsonObject objectins = documentins.object();
            QJsonValue valueins = objectins.value("data");
            int metaCodeIntins = objectins.value("meta").toObject().value("code").toInt();
            sdebug << "Meta Code" << metaCodeIntins;
            if(metaCodeIntins == 400)
            {
                sdebug << "meta Error";
                sdebug << tJSONins;
                reply->deleteLater();
                tManager->deleteLater();
                return;
            }
            objectins = valueins.toObject();
            valueins = objectins.value("target_user_is_private");
            if(!valueins.toBool())
            {
                QString currentUser =  mUserList.at(globalCounter);
                sdebug << "Non Priv" << currentUser;
                mNonPrivateUserList << currentUser;
            }
            ++globalCounter;
            reply->deleteLater();
            tManager->deleteLater();
            processNextUser();
        });

    } else {
        sdebug << "on else";
        pushButton_2->setText("Analize finished");
        nonPrivateCounter = 0;
        setNextNonPrivateUrl();
    }
}
void Window::processNextUserToUnfollow()
{
    if(globalCounter < mUserList.size())
    {
        pushButton_2->setText("Analizing Users...");
        QString currentUserName = mUserList.at(globalCounter);
        sdebug << "Current User Name" << currentUserName << globalCounter;
        QString userID = mUserIDList.at(globalCounter);
        QString url = "https://api.instagram.com/v1/users/"+ userID + "/relationship?&access_token=" + _MYTOKEN;
        QNetworkRequest request = QNetworkRequest(QUrl(url));
        QNetworkAccessManager* tManager = new QNetworkAccessManager;
        QNetworkReply* reply = tManager->get(request);
        connect(reply, &QNetworkReply::finished, [=](){
            QByteArray tJSONins = reply->readAll();
            QJsonDocument documentins = QJsonDocument::fromJson(tJSONins);
            QJsonObject objectins = documentins.object();
            QJsonValue valueins = objectins.value("data");
            int metaCodeIntins = objectins.value("meta").toObject().value("code").toInt();
            sdebug << "Meta Code" << metaCodeIntins;
            if(metaCodeIntins == 400)
            {
                sdebug << "meta Error";
                sdebug << tJSONins;
                reply->deleteLater();
                tManager->deleteLater();
                return;
            }
            objectins = valueins.toObject();
            valueins = objectins.value("incoming_status");
            if(!valueins.toBool())
            {
                QString currentUser =  mUserList.at(globalCounter);
                sdebug << "Non Priv" << currentUser;
                mNonFollowingUserList << currentUser;
            }
            ++globalCounter;
            reply->deleteLater();
            tManager->deleteLater();
            processNextUserToUnfollow();
        });

    } else {
        sdebug << "on else";
        pushButton_2->setText("Analize finished");
        nonFollowingCounter = 0;
        setNextNonFollowingUrl();
    }
}
void Window::clickFollowUnFollow()
{
    clickMouse(230,120);
    clickMouse(300,120);
    clickMouse(380,120);
}

void Window::likeClick()
{
    QKeyEvent * event1 = new QKeyEvent((QEvent::KeyPress), Qt::Key_End,Qt::NoModifier);
    QKeyEvent * event2 = new QKeyEvent((QEvent::KeyRelease), Qt::Key_End,Qt::NoModifier);
    qApp->postEvent((QObject*)webView, (QEvent*)event1);
    qApp->postEvent((QObject*)webView, (QEvent*)event2);
    clickMouse(30,225);
}
void Window::setNextNonPrivateUrl()
{
    if(nonPrivateCounter < mNonPrivateUserList.size()){
        pushButton_3->setText("Following " + QString::number(nonPrivateCounter+1) + "th user");
        followUser(mNonPrivateUserList.at(nonPrivateCounter));
        ++nonPrivateCounter;
    } else {
        pushButton_3->setText("Following finished");
        double delay = (30.0/50.0) * (double)nonPrivateCounter; //minutes
        delay = delay * 60; // seconds;
        setTimerUrl((int)delay);
        int msDelay = (int)delay * 1000; //msecs
        sdebug << msDelay;
        msDelay += 10;

        QTimer* timer = new QTimer;
        timer->setInterval(msDelay);
        timer->start();
        connect(timer, &QTimer::timeout, [=](){
            timer->stop();
            timer->deleteLater();
            if(likeCheckCounter>29)
            {
                likeCheckCounter = 0;
                checkRecentMedia();
            }
            else {
                obtainUsersToFollow();
                ++likeCheckCounter;
            }
        });
    }
}
void Window::setNextNonFollowingUrl()
{
    if(nonFollowingCounter < mNonFollowingUserList.size()){
        pushButton_3->setText("Unfollowing " + QString::number(nonFollowingCounter+1) + "th user");
        unfollowUser(mNonFollowingUserList.at(nonFollowingCounter));
        ++nonFollowingCounter;
    } else {
        pushButton_3->setText("Unfollowing finished");
        double delay = (30.0/50.0) * (double)nonFollowingCounter; //minutes
        delay = delay * 60; // seconds;
        setTimerUrl((int)delay);
        int msDelay = (int)delay * 1000; //msecs
        sdebug << msDelay;
        msDelay += 10;
        QTimer* timer = new QTimer;
        timer->setInterval(msDelay);
        timer->start();
        connect(timer, &QTimer::timeout, [=](){
            timer->stop();
            timer->deleteLater();
            timer->disconnect();
            if(likeCheckCounter>29)
            {
                likeCheckCounter = 0;
                checkRecentMedia();
            }
            else {
                obtainUsersToFollow();
                ++likeCheckCounter;
            }
        });
    }
}
void Window::renewPage()
{
    delete mPage;
    webView->page()->deleteLater();
    mPage = new QWebPage;
    mPage->settings()->setAttribute(QWebSettings::AutoLoadImages, false);
    mPage->networkAccessManager()->setCookieJar(new CookieJar);
    webView->setPage(mPage);
}
void Window::setTimerUrl(int delay)
{
    QDateTime dt = QDateTime::currentDateTime();
    dt = dt.addSecs(delay);
    QDate tDate = dt.date();
    QTime tTime = dt.time();
    setUrl(QUrl(mTimeCounterUrl + QString::number(tDate.year()) + "-" + QString::number(tDate.month()) + "-" + QString::number(tDate.day()) + "T" + tTime.toString()));
}
void Window::checkRecentMedia()
{
    pushButton->setText("Checking Media");
    QNetworkRequest request = QNetworkRequest(QUrl("https://api.instagram.com/v1/users/2103856233/media/recent/?access_token=" + _MYTOKEN + "&count=1"));
    QNetworkAccessManager* tManager = new QNetworkAccessManager;
    QNetworkReply* reply = tManager->get(request);
    connect(reply, &QNetworkReply::finished, [=](){
        QByteArray tJSON = reply->readAll();
        QJsonDocument document = QJsonDocument::fromJson(tJSON);
        QJsonObject object = document.object();
        if(object.value("meta").toObject().value("code").toInt() != 200)
            pushButton->setText("Meta Error in media check");
        else
        {
            QJsonValue value = object.value("data");
            QJsonArray array = value.toArray();
            if(array.size()>0)
            {
                object = array.at(0).toObject();
                if(!object.value("user_has_liked").toBool())
                {
                    pushButton->setText("Going to Like");
                    likePost(object.value("link").toString());
                } else {
                    if(followTrueUnfollowFalse)
                        obtainUsersToFollow();
                    else
                        obtainUsersToUnfollow();
                }
            } else
                pushButton->setText("Media size 0");
        }
        reply->deleteLater();
        tManager->deleteLater();
    });
}
void Window::on_pushButton_clicked()
{

}
void Window::on_pushButton_2_clicked()
{

}
void Window::on_pushButton_3_clicked()
{

}

