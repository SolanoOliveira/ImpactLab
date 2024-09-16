#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <msclr/marshal_cppstd.h>
#include <windows.h>

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace std;

string selectedImagePath;  // Caminho da imagem selecionada

// Função auxiliar para ler a imagem original no formato PPM
void readImage(const std::string &filename, std::vector<std::vector<std::vector<int>>> &imageData, int &width, int &height) {
    std::ifstream inputFile(filename);
    if (inputFile.is_open()) {
        std::string format;
        int maxVal;

        inputFile >> format;  // Lê o formato (deve ser "P3")
        inputFile >> width >> height;  // Lê as dimensões da imagem
        inputFile >> maxVal;  // Lê o valor máximo de cor (deve ser 255)

        // Lê os dados da imagem
        imageData.resize(height, std::vector<std::vector<int>>(width, std::vector<int>(3)));
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                inputFile >> imageData[y][x][0];  // Componente R
                inputFile >> imageData[y][x][1];  // Componente G
                inputFile >> imageData[y][x][2];  // Componente B
            }
        }
        inputFile.close();
    } else {
        MessageBox::Show("Erro ao abrir a imagem!", "Erro", MessageBoxButtons::OK, MessageBoxIcon::Error);
    }
}

// Função para exibir a imagem original no PictureBox
void displayImage(PictureBox^ pictureBox, const std::string &filePath) {
    Image^ image = Image::FromFile(gcnew String(filePath.c_str()));
    pictureBox->Image = image;
}

// Função para aplicar o filtro de bandeira e salvar a nova imagem
std::string applyFilterAndSave(std::string filename, float opacity = 0.5) {
    std::ofstream outputImage;
    outputImage.open("filtered_image.ppm");

    int width, height;
    std::vector<std::vector<std::vector<int>>> imageData;  // Armazena a imagem original
    readImage(filename, imageData, width, height);  // Lê a imagem original

    if (outputImage.is_open()) {
        outputImage << "P3" << std::endl;
        outputImage << width << " " << height << std::endl;
        outputImage << "255" << std::endl;  // Valor máximo de cor
    }

    // Define os limites para cada cor, distribuindo 1/6 da altura para cada cor
    int stripeHeight = height / 6;  // Aproximadamente 41 ou 42 pixels por faixa

    int R = 255, G = 0, B = 0;  // Cor inicial do filtro: vermelho

    // Aplica o filtro de bandeira sobre a imagem
    for (int y = 0; y < height; y++) {
        // Modifica a cor do filtro conforme a linha (baseado nas cores da bandeira)
        if (y < stripeHeight) {
            R = 255;
            G = 0;
            B = 0;  // Vermelho
        } else if (y < 2 * stripeHeight) {
            R = 255;
            G = 140;
            B = 0;  // Laranja
        } else if (y < 3 * stripeHeight) {
            R = 255;
            G = 237;
            B = 0;  // Amarelo
        } else if (y < 4 * stripeHeight) {
            R = 0;
            G = 128;
            B = 38;  // Verde
        } else if (y < 5 * stripeHeight) {
            R = 0;
            G = 76;
            B = 255;  // Azul
        } else {
            R = 115;
            G = 41;
            B = 130;  // Roxo
        }

        // Aplica o filtro de bandeira para cada pixel da linha atual
        for (int x = 0; x < width; x++) {
            // Cor original do pixel da imagem de fundo
            int original_R = imageData[y][x][0];
            int original_G = imageData[y][x][1];
            int original_B = imageData[y][x][2];

            // Calcula a nova cor após aplicar o filtro com opacidade
            int blended_R = (R * opacity) + (original_R * (1 - opacity));
            int blended_G = (G * opacity) + (original_G * (1 - opacity));
            int blended_B = (B * opacity) + (original_B * (1 - opacity));

            // Escreve a nova cor "blendada" no arquivo PPM
            outputImage << blended_R << " " << blended_G << " " << blended_B << std::endl;
        }
    }

    outputImage.close();
    return "filtered_image.ppm";  // Retorna o nome da imagem com filtro aplicado
}

// Função de callback para o botão "Buscar Imagem"
void onOpenImageClick(PictureBox^ pictureBox) {
    OpenFileDialog^ openFileDialog = gcnew OpenFileDialog();
    openFileDialog->Filter = "PPM Files|*.ppm|All Files|*.*";
    
    if (openFileDialog->ShowDialog() == DialogResult::OK) {
        selectedImagePath = msclr::interop::marshal_as<std::string>(openFileDialog->FileName);
        displayImage(pictureBox, selectedImagePath);  // Exibe a imagem original
    }
}

// Função de callback para o botão "Aplicar Filtro"
void onApplyFilterClick(PictureBox^ pictureBox) {
    if (!selectedImagePath.empty()) {
        std::string filteredImagePath = applyFilterAndSave(selectedImagePath);  // Aplica o filtro
        displayImage(pictureBox, filteredImagePath);  // Exibe a imagem com o filtro
    } else {
        MessageBox::Show("Nenhuma imagem selecionada!", "Erro", MessageBoxButtons::OK, MessageBoxIcon::Error);
    }
}

// Classe de formulário
public ref class MainForm : public Form {
private:
    Button^ openImageButton;
    Button^ applyFilterButton;
    PictureBox^ originalPictureBox;
    PictureBox^ filteredPictureBox;

public:
    MainForm() {
        // Inicializa os componentes do formulário
        this->Text = "Filtro de Imagem - WinForms";
        this->Size = Drawing::Size(800, 600);

        // Botão para abrir a imagem
        openImageButton = gcnew Button();
        openImageButton->Text = "Buscar Imagem";
        openImageButton->Location = Drawing::Point(50, 20);
        openImageButton->Click += gcnew EventHandler(this, &MainForm::openImageButton_Click);

        // Botão para aplicar o filtro
        applyFilterButton = gcnew Button();
        applyFilterButton->Text = "Aplicar Filtro";
        applyFilterButton->Location = Drawing::Point(200, 20);
        applyFilterButton->Click += gcnew EventHandler(this, &MainForm::applyFilterButton_Click);

        // PictureBox para exibir a imagem original
        originalPictureBox = gcnew PictureBox();
        originalPictureBox->Location = Drawing::Point(50, 70);
        originalPictureBox->Size = Drawing::Size(300, 300);
        originalPictureBox->SizeMode = PictureBoxSizeMode::StretchImage;

        // PictureBox para exibir a imagem com o filtro
        filteredPictureBox = gcnew PictureBox();
        filteredPictureBox->Location = Drawing::Point(400, 70);
        filteredPictureBox->Size = Drawing::Size(300, 300);
        filteredPictureBox->SizeMode = PictureBoxSizeMode::StretchImage;

        // Adiciona os componentes ao formulário
        this->Controls->Add(openImageButton);
        this->Controls->Add(applyFilterButton);
        this->Controls->Add(originalPictureBox);
        this->Controls->Add(filteredPictureBox);
    }

    // Eventos dos botões
    void openImageButton_Click(Object^ sender, EventArgs^ e) {
        onOpenImageClick(originalPictureBox);
    }

    void applyFilterButton_Click(Object^ sender, EventArgs^ e) {
        onApplyFilterClick(filteredPictureBox);
    }
};

// Função principal para executar o formulário
[STAThreadAttribute]
void Main() {
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    Application::Run(gcnew MainForm());
}
