#include <iostream>
#include <memory>
#include <string>
using namespace std;
// Implementação do padrão de projeto estrutural Proxy
// Referência: https://refactoring.guru/design-patterns/proxy
// Matheus Capitani Silveira RA: 2554739
// O padrão Proxy resolve o problema: "Como controlar/gerenciar acesso a um objeto real
// sem expor sua implementação direta ao cliente?"
// A solução proposta é criar uma classe intermediária (Proxy) que implementa a mesma interface
// do objeto real. O Proxy atua como um substituto, adicionando funcionalidades como:
// - Controle de acesso (ex: verificar permissões)
// - Logging de operações
// - Validação de estados (ex: conexão de rede)
// No exemplo abaixo, o Proxy é usado para gerenciar dispositivos IoT (lâmpadas smart).
// O DeviceProxy verifica se o usuário tem permissão de "Admin" antes de executar ações,
// registra logs de atividade e simula uma verificação de conexão de rede.
// Isso permite adicionar segurança e monitoramento sem modificar a classe LightBulb (objeto real).

// Interface para dispositivos IOT
class IDevice
{
  public:
    virtual ~IDevice() = default;
    virtual void turnOn() = 0;
    virtual void turnOff() = 0;
};

// Dispositivo real: Lâmpada Smart
class LightBulb : public IDevice
{
  private:
    string location;

  public:
    LightBulb(string loc) : location(loc)
    {
    }

    void turnOn() override
    {
        cout << "Lâmpada em " << location << " ligada!" << endl;
    }

    void turnOff() override
    {
        cout << "Lâmpada em " << location << " desligada!" << endl;
    }
};

// Proxy: Controla acesso à lâmpada com verificação de permissões
class DeviceProxy : public IDevice
{
  private:
    unique_ptr<LightBulb> realBulb;
    string userRole;
    bool isNetworkAvailable;

  public:
    DeviceProxy(string loc, string role)
        : realBulb(make_unique<LightBulb>(loc)), userRole(role), isNetworkAvailable(true)
    {
    }

    void checkAccess()
    {
        if (userRole != "Admin")
        {
            throw runtime_error("Acesso negado: usuário não é Admin!");
        }
        if (!isNetworkAvailable)
        {
            throw runtime_error("Erro de conexão com a lâmpada!");
        }
    }

    void turnOn() override
    {
        checkAccess();
        realBulb->turnOn();
        logAction("Ligar");
    }

    void turnOff() override
    {
        checkAccess();
        realBulb->turnOff();
        logAction("Desligar");
    }

    void logAction(const string &action)
    {
        cout << "[LOG] Ação '" << action << "' registrada." << endl;
    }
};

int main()
{
    try
    {
        // Cria um proxy para a lâmpada da sala (usuário comum)
        auto livingRoomLight = make_unique<DeviceProxy>("Sala", "Convidado");
        livingRoomLight->turnOn(); // Lança exceção: acesso negado
    }
    catch (const exception &e)
    {
        cout << "Erro: " << e.what() << endl;
    }

    // Cria um proxy para a lâmpada do quarto (usuário Admin)
    auto bedroomLight = make_unique<DeviceProxy>("Quarto", "Admin");
    bedroomLight->turnOn(); // Funciona
    bedroomLight->turnOff();

    return 0;
}
