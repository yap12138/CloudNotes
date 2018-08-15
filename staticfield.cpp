#include <iostream>

using std::cout;
using std::endl;

class Widget;   //forward declaration;

//non-member, non-friend function.
std::ostream& operator<<(std::ostream& out, const Widget& widget);

class Widget
{
    static Widget _widget;
    explicit Widget(int val = 7) : _val(val) {}
public:
    static void show() {
        cout<<_widget<<endl;
    }

    static Widget& getInstance() { return _widget; }

    int getValue() const { return _val; }
private:
    int _val;
};

std::ostream& operator<<(std::ostream& out, const Widget& widget) {
    out<<"this is widget object: "<<widget.getValue();
    return out;
}

Widget Widget::_widget(77);

int main(int argc, char const *argv[])
{
    Widget::show();
    //cout<<Widget::getInstance()<<endl;
    return 0;
}
