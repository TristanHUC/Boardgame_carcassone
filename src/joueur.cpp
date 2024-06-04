#include "joueur.hpp"

#include <map>

#include "comboboxdialog.h"
#include "ui_comboboxdialog.h"

std::unique_ptr<Pion> Joueur::selectionnerPion()  {

    ComboBoxDialog dial;

    std::map<std::string, std::size_t> types;

    for (auto& pion: pions_) {
        types[pion->type()] = typeid(*pion).hash_code();
    }

    for (const auto& t: types) {
        dial.getUi()->comboBox->addItem(QString::fromStdString(t.first));
    }

    const auto res = dial.exec();

    if (res == QDialog::Accepted) {
        if (nbpionrestant_ != 0) {
            --nbpionrestant_;
            auto it = std::find_if(pions_.begin(), pions_.end(), [&](auto& p){
                return typeid(*p).hash_code() == types[dial.getUi()->comboBox->currentText().toStdString()];
            });

            if (it == pions_.end()) {
                return nullptr;
            }

            auto pion = std::move(*it);
            pions_.erase(it);
            return pion;
        }
    }
    return nullptr;
}

unsigned long Joueur::findIndex(const std::vector<std::unique_ptr<Pion>>& arr, int item) {
  for (unsigned long i = 0; i < arr.size(); ++i) {
    if (arr[i]->getNumero() == item) return i;
  }
  return -1;
}
