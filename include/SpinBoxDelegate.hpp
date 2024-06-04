#ifndef SPINBOXDELEGATE_HPP
#define SPINBOXDELEGATE_HPP

#include <QSpinBox>
#include <QStyledItemDelegate>

class SpinBoxDelegate : public QStyledItemDelegate {

    Q_OBJECT

public:
    SpinBoxDelegate(QObject* parent = nullptr) : QStyledItemDelegate(parent) {}

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override {

        QSpinBox* editor = new QSpinBox(parent);
        editor->setFrame(false);
        editor->setMinimum(0);
        editor->setMaximum(200);

        return editor;
    }

    void setEditorData(QWidget* editor, const QModelIndex& index) const override {

        const auto value = index.model()->data(index, Qt::EditRole).toInt();

        auto spinBox = static_cast<QSpinBox*>(editor);
        spinBox->setValue(value);
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override {

        auto spinBox = static_cast<QSpinBox*>(editor);
        spinBox->interpretText();
        const auto value = spinBox->value();

        model->setData(index, value, Qt::EditRole);
    }

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex& /*index*/) const override {
        editor->setGeometry(option.rect);
    }
};

#endif // SPINBOXDELEGATE_HPP
