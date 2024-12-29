#include "mainwindow.h"

#include <QApplication>
#include <QTreeView>
#include <QFileSystemModel>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QPushButton>

class FileExplorer : public QWidget {
    Q_OBJECT

public:
    FileExplorer(QWidget *parent = nullptr) : QWidget(parent) {
        // Set up the file system model
        QFileSystemModel *model = new QFileSystemModel(this);
        model->setRootPath("");

        // Set up the tree view
        QTreeView *treeView = new QTreeView(this);
        treeView->setModel(model);
        treeView->setRootIndex(model->index("."));
        treeView->setColumnWidth(0, 250);

        // Set up the path navigation bar
        QHBoxLayout *pathLayout = new QHBoxLayout();
        QLabel *pathLabel = new QLabel("Path:", this);
        QHBoxLayout *foldersLayout = new QHBoxLayout();

        connect(treeView->selectionModel(), &QItemSelectionModel::selectionChanged, [=](const QItemSelection &selected) {
            if (!selected.indexes().isEmpty()) {
                QModelIndex index = selected.indexes().first();
                QString filePath = model->filePath(index);

                // Clear the old navigation buttons
                QLayoutItem *child;
                while ((child = foldersLayout->takeAt(0)) != nullptr) {
                    delete child->widget();
                    delete child;
                }

                // Split the file path and add buttons with slashes for navigation
                QStringList pathComponents = filePath.split("/");
                QString currentPath;
                for (int i = 0; i < pathComponents.size(); ++i) {
                    const QString &component = pathComponents[i];
                    if (component.isEmpty()) continue;
                    currentPath += "/" + component;

                    QPushButton *folderButton = new QPushButton(component, this);
                    folderButton->setFlat(true);
                    foldersLayout->addWidget(folderButton);

                    connect(folderButton, &QPushButton::clicked, [=]() {
                        QModelIndex newIndex = model->index(currentPath);
                        treeView->setRootIndex(newIndex);
                    });

                    // Add a slash after each folder except the last one
                    if (i < pathComponents.size() - 1) {
                        QLabel *slashLabel = new QLabel("/", this);
                        foldersLayout->addWidget(slashLabel);
                    }
                }
            }
        });

        pathLayout->addWidget(pathLabel);
        pathLayout->addLayout(foldersLayout);

        // Set up a label to show the selected file path
        QLabel *filePathLabel = new QLabel("Selected Path:", this);
        QLineEdit *filePathEdit = new QLineEdit(this);
        filePathEdit->setReadOnly(true);

        connect(treeView->selectionModel(), &QItemSelectionModel::selectionChanged, [=](const QItemSelection &selected) {
            if (!selected.indexes().isEmpty()) {
                QModelIndex index = selected.indexes().first();
                QString filePath = model->filePath(index);
                filePathEdit->setText(filePath);
            }
        });

        // Set up the layout
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addLayout(pathLayout);
        layout->addWidget(treeView);
        layout->addWidget(filePathLabel);
        layout->addWidget(filePathEdit);
        setLayout(layout);

        // Set window properties
        setWindowTitle("Orca");
        resize(800, 600);
    }
};

#include "main.moc"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    FileExplorer fileExplorer;
    fileExplorer.show();

    return app.exec();
}


