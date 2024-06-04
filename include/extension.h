#ifndef EXTENSION_H
#define EXTENSION_H

#include <memory>
#include <fstream>
#include <string>
#include <typeinfo>
#include <QFile>
#include <QTextStream>

#include "pion.h"
#include "pion.h"
#include "tuile.h"

//test
class Extension {
 public:
    std::vector<std::unique_ptr<Pion>> creerPions();
    void initialisation(const string& s);
    std::vector<Tuile*> getTuiles() {
        std::vector<Tuile*> res;
        res.reserve(tuiles.size());
        for (auto& t: tuiles) {
            res.push_back(t.get());
        }
        return res;
    }
 private:
    std::unique_ptr<Tuile> fabriqueTuile(const std::array<string, 13>&, const std::vector<std::vector<unsigned int>>&, const string&,
                         bool blason, bool auberge, bool cathedrale) const;
    virtual std::vector<std::unique_ptr<Pion>> doCreerPions()=0;
 protected:
    std::vector<std::unique_ptr<Tuile>> tuiles;
};

class Base : public Extension {
 public:
    Base() {
        initialisation(":/Tuile_base.txt");
    }
  Base(const Base&) = delete;
  Base& operator=(const Base&) = delete;

 private:
  std::vector<std::unique_ptr<Pion>> doCreerPions() override;
};

class Rivieres : public Extension {
 public:
    Rivieres() {
        initialisation(":/Tuile_riviere.txt");
    }
  Rivieres(const Rivieres&) = delete;
  Rivieres& operator=(const Rivieres&) = delete;

 private:
  std::vector<std::unique_ptr<Pion>> doCreerPions() override {
      return {};
  }
};

class Auberges : public Extension {
 public:
    Auberges(){
        initialisation(":/Tuile_auberge.txt");
    }
  Auberges(const Auberges&) = delete;
  Auberges& operator=(const Auberges&) = delete;

 private:
  std::vector<std::unique_ptr<Pion>> doCreerPions() override;
};

class Abbes : public Extension {
 public:
  Abbes(){
      initialisation(":/Tuile_abbe.txt");
  }
  Abbes(const Abbes&) = delete;
  Abbes& operator=(const Abbes&) = delete;

 private:
  std::vector<std::unique_ptr<Pion>> doCreerPions() override;
};

class Paysans : public Extension {
 public:
  Paysans() = default;
  Paysans(const Paysans&) = delete;
  Paysans& operator=(const Paysans&) = delete;

 private:
  std::vector<std::unique_ptr<Pion>> doCreerPions() override {
      return {};
  }
};

#endif  // EXTENSION_H
