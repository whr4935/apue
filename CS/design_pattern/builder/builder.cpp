#include <iostream>
#include <string>
#include <memory>
using namespace std;

class Pizza 
{
public:
    void setDough(const string& dough)
    {
        mDough = dough;
    }

    void setSauce(const string& sauce)
    {
        mSauce = sauce;
    }

    void setTopping(const string& topping)
    {
        mTopping = topping;
    }

    void open() const
    {
        cout << "Pizza with " << mDough << " dough, " << mSauce << " sauce and "
             << mTopping << " topping. Mmm." << std::endl;
    }

private:
    string mDough;
    string mSauce;
    string mTopping;
};

//Abstract builder
class PizzaBuilder
{
public:
    virtual ~PizzaBuilder() {}

    Pizza* getPizza() {
        return mPizza.get();
    }

    void createNewPizzaProduct() {
        mPizza = make_unique<Pizza>();
    }

    virtual void buildDough() = 0;
    virtual void buildSauce() = 0;
    virtual void buildTopping() = 0; 

protected:
    unique_ptr<Pizza> mPizza;
};

class HawaiianPizzaBuilder : public PizzaBuilder
{
public:
    virtual ~HawaiianPizzaBuilder() {}
    virtual void buildDough() {
        mPizza->setDough("cross");
    }

    virtual void buildSauce() {
        mPizza->setSauce("mild");
    }

    virtual void buildTopping() {
        mPizza->setTopping("ham+pineapple");
    }
};

class SpicyPizzaBuilder : public PizzaBuilder
{
public:
    virtual ~SpicyPizzaBuilder() {}
    virtual void buildDough() {
        mPizza->setDough("pan baked");
    }

    virtual void buildSauce() {
        mPizza->setSauce("hot");
    }

    virtual void buildTopping() {
        mPizza->setTopping("pepperoni+salami");
    }
};

class Cook
{
public:
    void openPizza() {
        mPizzaBuilder->getPizza()->open();
    }

    void makePizza(PizzaBuilder* pb) {
        mPizzaBuilder = pb;
        mPizzaBuilder->createNewPizzaProduct();
        mPizzaBuilder->buildDough();
        mPizzaBuilder->buildSauce();
        mPizzaBuilder->buildTopping();
    }

private:
    PizzaBuilder* mPizzaBuilder;
};

int main(int argc, char *argv[])
{
    Cook cook;
    HawaiianPizzaBuilder hawaiianPizzaBuilder;
    SpicyPizzaBuilder spicyPizzaBuilder;
    cook.makePizza(&hawaiianPizzaBuilder);
    cook.openPizza();

    cook.makePizza(&spicyPizzaBuilder);
    cook.openPizza();

    return 0;
}
