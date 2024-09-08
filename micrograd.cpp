#include <iostream>
#include <vector>
#include <set>
#include <cmath>
#include <functional>
#include <memory>

// Define the Value class
class Value {
public:
    double data;
    double grad;
    std::set<Value*> _prev;  // The set of parent nodes (children in autograd)
    std::string _op;         // The operation that produced this node
    std::function<void()> _backward;  // A function to compute the gradient during backpropagation

    // Constructor
    Value(double data, const std::set<Value*>& children = {}, const std::string& op = "")
        : data(data), grad(0), _prev(children), _op(op), _backward([]() {}) {}

    // Operator overloading for addition
    Value operator+(const Value& other) {
        Value out(this->data + other.data, {this, const_cast<Value*>(&other)}, "+");

        // Define the backward function
        out._backward = [this, &other, &out]() {
            this->grad += out.grad;
            const_cast<Value&>(other).grad += out.grad;
        };

        return out;
    }

    // Operator overloading for multiplication
    Value operator*(const Value& other) {
        Value out(this->data * other.data, {this, const_cast<Value*>(&other)}, "*");

        // Define the backward function
        out._backward = [this, &other, &out]() {
            this->grad += other.data * out.grad;
            const_cast<Value&>(other).grad += this->data * out.grad;
        };

        return out;
    }

    // Operator overloading for power (only supports int/float exponents)
    Value operator^(double exponent) {
        Value out(std::pow(this->data, exponent), {this}, "**" + std::to_string(exponent));

        // Define the backward function
        out._backward = [this, exponent, &out]() {
            this->grad += (exponent * std::pow(this->data, exponent - 1)) * out.grad;
        };

        return out;
    }

    // ReLU activation function
    Value relu() {
        Value out(this->data < 0 ? 0 : this->data, {this}, "ReLU");

        // Define the backward function
        out._backward = [this, &out]() {
            this->grad += (out.data > 0) * out.grad;
        };

        return out;
    }

    // Backpropagation: computes the gradient
    void backward() {
        // Topological sort (DFS) to order the nodes
        std::vector<Value*> topo;
        std::set<Value*> visited;

        // Recursive helper function for DFS
        std::function<void(Value*)> build_topo = [&](Value* v) {
            if (!visited.count(v)) {
                visited.insert(v);
                for (auto child : v->_prev) {
                    build_topo(child);
                }
                topo.push_back(v);
            }
        };

        // Build the topological order
        build_topo(this);

        // Start backpropagation
        this->grad = 1;  // Start with gradient 1 for the output
        for (auto it = topo.rbegin(); it != topo.rend(); ++it) {
            (*it)->_backward();
        }
    }

    // Operator overloading for unary minus (-self)
    Value operator-() {
        // return *this * -1;
        Value out(this->data * -1, {this}, "-1");
        return out;
    }

    // Operator overloading for subtraction
    Value operator-(const Value& other) {
        Value out(this->data - other.data, {this, const_cast<Value*>(&other)}, "-");
        return out;
    }

    // Operator overloading for division
    Value operator/(const Value& other) {
        Value out(this->data / (other.data), {this, const_cast<Value*>(&other)}, "/");
        // return *this * (other ^ -1);
        return out;
    }

    // Helper to print the Value object
    friend std::ostream& operator<<(std::ostream& os, const Value& v) {
        os << "Value(data=" << v.data << ", grad=" << v.grad << ")";
        return os;
    }
};

int main() {
    Value a(2.0);
    Value b(3.0);

    // perform operations
    Value c = a + b;
    Value d = a * b;
    Value e = d ^ 2;
    Value f = e.relu();

    std::cout << "Result: " << f << std::endl;
    
    // backproping to calculate gradients
    // f.backward();

    // std::cout << "Gradient of a: " << a << std::endl;
    // std::cout << "Gradient of b: " << b << std::endl;

} 