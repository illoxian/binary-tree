#ifndef COMPARATORE_H
#define COMPARATORE_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QtDebug>
#include <QStyle>
#include <QDesktopWidget>
#include <QtCharts/QChartView>

#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>



QT_BEGIN_NAMESPACE
namespace Ui { class Comparatore; }

QT_END_NAMESPACE


typedef QList<int> IntList;
typedef QList<IntList> IntTable;

class Comparatore : public QWidget
{
    Q_OBJECT

public:
    Comparatore(QWidget *parent = nullptr);
    ~Comparatore();
private:

void request();



private slots:
  void fillBoxes();
  // slot per seganle finished()
  void dataReadyEmi();
  void dataReadyUni();





  void on_emiButton_clicked();
  void on_uniButton_clicked();




  void on_graficiButton_clicked();

private:
    Ui::Comparatore *ui;
    QNetworkReply *replyEmi;
    QNetworkReply *replyUni;
    QNetworkAccessManager *manager;
    QString textEmi;
    QString textUni;
    QUrl emiUrl;
    QUrl uniUrl;
    QStringList list;
    QStringList list2;

    int artistiUniveral;
    int artistiEmi;



};
#endif // COMPARATORE_H
