#include "comparatore.h"

#include "ui_comparatore.h"
#include <QMessageBox>
#include <iostream>





Comparatore::Comparatore(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Comparatore)
{
    ui->setupUi(this);
    //this->resize(800, 700);
    ui->graficiButton->setDefault(true);
    ui->graficiButton->setAutoDefault(true);
    ui->graficiButton->setEnabled(true);

    emiUrl = "http://www.ivl.disco.unimib.it/minisites/cpp/List_of_EMI_artists.txt";
    uniUrl = "http://www.ivl.disco.unimib.it/minisites/cpp/List_of_Universal_artists.txt";
    fillBoxes();



}

Comparatore::~Comparatore()
{
    delete ui;
}


// a run time parte il fill dei campi di testo
void Comparatore::fillBoxes() {
  manager = new QNetworkAccessManager(this);
  request();
}

// parte la richiesta dei file txt dai due url
// una volta ricevuta emette il segnale finished collegato ai due dataReady**()
void Comparatore::request() {

  replyEmi = manager->get(QNetworkRequest(emiUrl));
  replyUni = manager->get(QNetworkRequest(uniUrl));

  connect(replyEmi, SIGNAL(finished()),
      this, SLOT(dataReadyEmi()));
  connect(replyUni, SIGNAL(finished()),
      this, SLOT(dataReadyUni()));
}



// lavora sul testo scaricato un volta pronto -- chiamato da SIGNAL finished()
// lo formatta e genera i link wikipedia
void Comparatore::dataReadyEmi() {

  if(replyEmi->error()){
    QMessageBox replyError;
    replyError.setText("<font color=\"red\"> impossibile reperire i dati, problema di connessione </font>");
    replyError.exec();
  }else{
      textEmi = QString(replyEmi->readAll());
      replyEmi = nullptr;
      list = textEmi.split('\n');
      textEmi = "";
      artistiEmi = list.length();

      for (int i=0; i<list.length(); i++) {
          int j=0;
          QString curr = list[i];
          j = curr.mid(1).lastIndexOf('"');
          list[i]=curr.mid(j+3,1);
          list[i].append(" ");


          textEmi.append("<a href=");
          if (curr.lastIndexOf("http")!=-1) {
             textEmi.append(curr.mid(1,j));
          }
          else if(curr.lastIndexOf("wiki")!=-1) {
             textEmi.append("https://en.wikipedia.org");
             textEmi.append(curr.mid(1,j));
          }
          textEmi.append(">").append(curr.mid(j+3)).append("_</a><br>");


      }
      ui->textboxEmi->setOpenExternalLinks(true);
      ui->textboxEmi->setText(textEmi);
      textEmi="";
  }
}

// lavora sul testo scaricato un volta pronto -- chiamato da SIGNAL finished()
// lo formatta e genera i link wikipedia
void Comparatore::dataReadyUni() {

  if(replyUni->error()){
    QMessageBox replyError;
    replyError.setText("<font color=\"red\"> impossibile reperire i dati, problema di connessione </font>");
    replyError.exec();
  }else{
      textUni = QString(replyUni->readAll());
      replyUni = nullptr;
      list2 = textUni.split('\n');
      textUni = "";
      artistiUniveral = list2.length();

      for (int i=0; i<list2.length(); i++) {
          int j=0;
          QString curr = list2[i];
          j = curr.mid(1).lastIndexOf('"');
          list2[i]=curr.mid(j+3,1);
          list2[i].append(" ");
          textUni.append("<a href=");
          if (curr.lastIndexOf("http")!=-1) {
             textUni.append(curr.mid(1,j));
          }
          else if(curr.lastIndexOf("wiki")!=-1) {
             textUni.append("https://en.wikipedia.org");
             textUni.append(curr.mid(1,j));
          }
          textUni.append(">").append(curr.mid(j+3)).append("</a><br>");


      }
      ui->textboxUni->setOpenExternalLinks(true);
      ui->textboxUni->setText(textUni);
      textUni="";
  }
}

// chiamata al click del bottone Emi
// genera a run time il grafico sui dati disponibili in quel momento nel textBox

void Comparatore::on_emiButton_clicked() {
    char base='A';
    QtCharts::QBarSeries *emiBarre = new QtCharts::QBarSeries();

    int othersum=-26;
    for (int i = 0; i<26; i++) {
        int sum =0;
        int j=0;
        for (j=0; j<list.length(); j++) {
            if(list[j].at(0)==QChar(base+i)) {
                sum++;
            }
        }
        if (j==list.length()) othersum+=1;

        QtCharts::QBarSet *set = new QtCharts::QBarSet(QString(base+i));
        *set << sum;
        emiBarre->append(set);
    }
    QtCharts::QBarSet *set0 = new QtCharts::QBarSet("0#!");
    *set0 << othersum;
    emiBarre->append(set0);


    QtCharts::QChart *chart = new QtCharts::QChart();

    chart->addSeries(emiBarre);
    chart->createDefaultAxes();
    chart->setTitle("Ripartizione per lettera artisti EMI");
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);


    QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

    chartView->setWindowTitle("Numero artisti EMI");
    chartView->resize(500,500);
    chartView->show();




}
void Comparatore::on_uniButton_clicked() {
    char base='A';
    QtCharts::QBarSeries *uniBarre = new QtCharts::QBarSeries();

    int othersum=-26;
    for (int i = 0; i<26; i++) {
        int sum =0;
        int j=0;
        for (j=0; j<list2.length(); j++) {
            if(list2[j].at(0)==QChar(base+i)) {
                sum++;
            }
        }
        if (j==list2.length()) othersum+=1;

        QtCharts::QBarSet *set = new QtCharts::QBarSet(QString(base+i));
        *set << sum;
        uniBarre->append(set);
    }
    QtCharts::QBarSet *set0 = new QtCharts::QBarSet("0#!");
    *set0 << othersum;
    uniBarre->append(set0);

    QtCharts::QChart *chart = new QtCharts::QChart();

    chart->addSeries(uniBarre);
    chart->createDefaultAxes();
    chart->setTitle("Ripartizione per lettera artisti UNIVERSAL");
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);


    QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);

    chartView->setWindowTitle("Numero artisti UNIVERSAL");
    chartView->resize(500,500);
    chartView->show();


}

// genera la chart di comparazione basandosi sulle variabili globali aggiornate durante il processamento del testo nella textbox
void Comparatore::on_graficiButton_clicked() {
    QtCharts::QPieSeries *comparaTorta = new QtCharts::QPieSeries();

    comparaTorta->append("Emi",artistiEmi);
    comparaTorta->append("Universal",artistiUniveral);

    QtCharts::QPieSlice *slice0 = comparaTorta->slices().at(0);
    slice0->setLabelVisible();

    QtCharts::QPieSlice *slice1 = comparaTorta->slices().at(1);
    slice1->setLabelVisible();

    QtCharts::QChart *chart = new QtCharts::QChart();

    chart->addSeries(comparaTorta);
    chart->setTitle("<center>Numero artisti per casa discografica <br>"
                    "Universal: " + QString::number(artistiUniveral) +
                    " vs Emi: " + QString::number(artistiEmi) + "</center>");

    chart->legend()->hide();

    QtCharts::QChartView *chartView = new QtCharts::QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    chartView->setWindowTitle("Numero artisti per casa discografica");
    chartView->resize(500,500);
    chartView->show();
}
