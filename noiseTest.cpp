#include "noiseTest.h"
#include "simplex2D.h"
#include "perlinNoise.h"
#include "utility.h"

#include <QApplication>
#include <QLabel>
#include <QMainWindow>
#include <QMenuBar>
#include <QStatusBar>
#include <QWidget> 
#include <limits>
#include <QMessageBox>

#include <QDebug>

noiseTest::noiseTest(uint32_t w, uint32_t h, QWidget *parent)  : QMainWindow(parent), m_width(w), m_height(h)
{
    setupUI();
    setupActions();
    setupMenus();
}


noiseTest::~noiseTest()
{
  if(nullptr != m_noise)
    delete[] m_noise;

  if (nullptr != m_pnoise)
    delete m_pnoise;
}


void noiseTest::setupUI()
{
    QWidget*      centralWidget;
    QMenuBar*     menuBar;
    QStatusBar*   statusBar;

    if (this->objectName().isEmpty())
        this->setObjectName(QString::fromUtf8("noiseTestClass"));
    this->resize(323, 331);

    centralWidget = new QWidget(this);
    centralWidget->setObjectName(QString::fromUtf8("centralWidget"));

    m_label = new QLabel(centralWidget);
    m_label->setObjectName(QString::fromUtf8("m_label"));
    m_label->setGeometry(QRect(30, 10, m_width, m_height));
    m_label->setFrameShape(QFrame::Box);

    this->setCentralWidget(centralWidget);
    menuBar = new QMenuBar(this);

    menuBar->setObjectName(QString::fromUtf8("menuBar"));
    menuBar->setGeometry(QRect(0, 0, 323, 21));
    this->setMenuBar(menuBar);


    statusBar = new QStatusBar(this);
    statusBar->setObjectName(QString::fromUtf8("statusBar"));
    this->setStatusBar(statusBar);
}

void noiseTest::setupActions()
{
  m_fileNew = new QAction("New", this);
  m_fileNew->setShortcuts(QKeySequence::New);
  m_fileNew->setStatusTip("Generates a new map");
  connect(m_fileNew, &QAction::triggered, this, &noiseTest::onFileNew);

  m_fileExit = new QAction("Exit", this);
  m_fileExit->setShortcuts(QKeySequence::Quit);
  m_fileExit->setStatusTip("Exit application");
  connect(m_fileExit, &QAction::triggered, this, &noiseTest::onFileExit);

  m_editSelPerlin = new QAction("Use Perlin Noise", this);
  m_editSelPerlin->setStatusTip("Use Perlin Noise for generation");
  m_editSelPerlin->isCheckable();
  m_editSelPerlin->setChecked(false);
  connect(m_editSelPerlin, &QAction::triggered, this, &noiseTest::onUsePerlin);

  m_editSelSimplex = new QAction("Use Simplex Noise", this);
  m_editSelSimplex->setStatusTip("Use Simplex Noise for generation");
  m_editSelSimplex->isCheckable();
  m_editSelSimplex->setChecked(false);
  connect(m_editSelSimplex, &QAction::triggered, this, &noiseTest::onUseSimplex);


}

void noiseTest::setupMenus()
{
  QMenu* fileMenu = menuBar()->addMenu("File");
  fileMenu->addAction(m_fileNew);
  fileMenu->addSeparator();
  fileMenu->addAction(m_fileExit);

  QMenu* editMenu = menuBar()->addMenu("Edit");
  QMenu* methodMenu = editMenu->addMenu("method");
  methodMenu->addAction(m_editSelPerlin);
  methodMenu->addAction(m_editSelSimplex);
}

void noiseTest::onFileNew()
{
  float minNoise = 1.00;

  if (m_method == noiseTest::method::PERLIN)
  {
    // only instantiate a new noise class if needed  -- if pnoise exists and old method was not perlin
    if ((m_pnoise != nullptr) && (m_oldMethod != noiseTest::method::PERLIN))
    { 
      delete m_pnoise;
      m_pnoise = new BasicPerlinNoise;
    }

    if (m_pnoise == nullptr)
    {
      m_pnoise = new BasicPerlinNoise;
    }
    
    m_oldMethod = noiseTest::method::PERLIN;
  }
  else if (m_method == noiseTest::method::SIMPLEX)
  {
    // only instantiate a new noise class if needed  -- if pnoise exists and old method was not simplex
    if ((m_pnoise != nullptr) && (m_oldMethod != noiseTest::method::SIMPLEX))
    {
      delete m_pnoise;
      m_pnoise = new simplex2D;
    }

    if(m_pnoise == nullptr)
    {
      m_pnoise = new simplex2D;
    }

    m_oldMethod = noiseTest::method::SIMPLEX;
  }
  else
  {
    QMessageBox::warning(this, "warning", "no noise method selected");
    return;
  }

  uint64_t max = std::numeric_limits<int32_t>::max() - 1;
  m_pnoise->reseed(Random(max));     // reshuffle noise array

  if (nullptr != m_noise)                                            // delete old noise data, if any
  {
    delete[] m_noise;
    m_noise = nullptr;
  }

  m_noise = new double[m_width * m_height];     // generate new noise array
  for (uint32_t ndx = 0; ndx < m_width * m_height; ndx++)
    m_noise[ndx] = 0;

  for (int row = 0; row < m_height; row++)      // sample noise array
  {
    for (int col = 0; col < m_width; col++)
    {
      double x = (double)col / ((double)100);
      double y = (double)row / ((double)100);

      //m_noise[col + row * m_width] = perlin.octave2D(x, y, 6);
      m_noise[col + row * m_width] = m_pnoise->getNoise(x, y, 6);
      if (m_noise[col + row * m_width] < minNoise) minNoise = m_noise[col + row * m_width];
    }
  }

  
  QImage img(m_width, m_height, QImage::Format_RGB888);
  img.fill(QColor(Qt::white).rgb());

  for (int row = 0; row < m_height; row++)
  {
    for (int col = 0; col < m_width; col++)
    {
      QRgb color;
      //int color = (int)((m_noise[col + row * m_width] + minNoise) * 255);                    // generate a gray-scale value between 0 and 255
      float val = m_noise[col + row * m_width];
      if (val <= -0.05) color = qRgb(0, 0, 255);
      else if ((val > -0.05) && val <= 0.05) color = qRgb(252, 194, 3);              // yellow
      else if ((val > 0.05) && (val <= 0.35)) color = qRgb(0, 255, 0);
      else if ((val > 0.35) && (val <= 0.75)) color = qRgb(102, 51, 0);             //  brown
      else color = qRgb(255, 255, 255);

      //img.setPixel(col, row, qRgb(color, color, color));
      img.setPixel(col, row, color);
    }
  }

  m_label->setPixmap(QPixmap::fromImage(img));
  m_label->show();
}

void noiseTest::onFileExit()
{
  QApplication::exit();
}

void noiseTest::onUsePerlin()
{
  m_editSelPerlin->setChecked(true);
  m_editSelSimplex->setChecked(false);

  m_method = noiseTest::method::PERLIN;
}


void noiseTest::onUseSimplex()
{
  m_editSelPerlin->setChecked(false);
  m_editSelSimplex->setChecked(true);

  m_method = noiseTest::method::SIMPLEX;
}
