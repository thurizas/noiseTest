#ifndef _noiseTest_h_
#define _noiseTest_h_

#include <QMainWindow>

#include <cstdint>

#include "noise.h"

class QLabel;
class QAction;

class noiseTest : public QMainWindow
{
    Q_OBJECT

public:
  enum method : std::uint8_t { UNDEFINED = 0, PERLIN = 1, SIMPLEX = 2 };

    noiseTest(uint32_t, uint32_t, QWidget *parent = Q_NULLPTR);
    ~noiseTest();

private slots:
  void onFileNew();
  void onFileExit();
  void onUsePerlin();
  void onUseSimplex();

private:
    //Ui::noiseTestClass ui;
    void setupUI();
    void setupMenus();
    void setupActions();

    QAction* m_fileNew;
    QAction* m_fileExit;
    QAction* m_editSelPerlin;
    QAction* m_editSelSimplex;
    
    uint32_t  m_width = 0;
    uint32_t  m_height = 0;
    QLabel*   m_label = nullptr;
    
    double* m_noise = nullptr;;
    uint8_t m_method = noiseTest::method::UNDEFINED;
    uint8_t m_oldMethod = noiseTest::method::UNDEFINED;

    noise* m_pnoise = nullptr;
};

#endif
