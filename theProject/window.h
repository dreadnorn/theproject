#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QUrl>
#include <QWebElement>
#include "ui_window.h"
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QThread>
#include <QDebug>
#include <QTimer>
#include "cookiejar.h"
#include "userinfo.h"

#define sdebug qDebug() << __FILE__ << __FUNCTION__ << __LINE__
QT_BEGIN_NAMESPACE
class QTreeWidgetItem;
QT_END_NAMESPACE


class Window : public QMainWindow, private Ui::Window
{
    Q_OBJECT

public:
    Window(QWidget *parent = 0);
    void setUrl(const QUrl &url);

private slots:
    void on_pushButton_clicked();
    void onProgress(int value);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();


private:
    bool followTrigerred;
    bool unfollowTrigerred;
    bool likeTrigerred;
    bool isLoggedIn;
    bool followTrueUnfollowFalse;
    QStringList mUserList;
    QStringList mUserIDList;
    QStringList mNonPrivateUserList;
    QStringList mNonFollowingUserList;
    QString _PASSWORD;
    QString mPaginationNextUrl;
    QString mTimeCounterUrl;
    int globalCounter;
    int nonPrivateCounter;
    int nonFollowingCounter;
    int likeCheckCounter;
    QWebPage* mPage;


    bool isLoginSuccessfull(QString plainWeb);
    bool isFollowing(QString plainWeb);
    void login();
    void clickMouse(int x, int y);
    void goToUserUrl(QString userName);
    void followUser(QString userNameToFollow);
    void unfollowUser(QString userNameToUnfollow);
    void likePost(QString url);
    void obtainUsersToFollow();
    void obtainUsersToUnfollow();
    void getUserID(QString userName);
    void getTargetUserID();
    void processNextUser();
    void processNextUserToUnfollow();
    void clickFollowUnFollow();
    void likeClick();
    void setNextNonPrivateUrl();
    void setNextNonFollowingUrl();
    void renewPage();
    void setTimerUrl(int delay);
    void checkRecentMedia();

signals:
    void s_userID(QString);
};

#endif
