var Stack = (function () {
    let items = new WeakMap();
    class Stack {
        constructor() {
            items.set(this, []);
        }

        push(element) {
            let s = items.get(this);
            s.push(element);
        }

        pop() {
            let s = items.get(this);
            return s.pop();
        }

        peek() {
            let s = items.get(this);
            return s[s.length - 1];
        }

        isEmpty() {
            let s = items.get(this);
            return s.length == 0;
        }

        size() {
            let s = items.get(this);
            return s.length;
        }

        clear() {
            let s = items.get(this);
            s = [];
        }

        get(idx) {
            let s = items.get(this);
            if (idx < 0 || idx >= s.length) {
                return null;
            } else {
                return s[idx];
            }
        }

        print() {
            let s = items.get(this);
            console.log(s.toString());
        }
    }

    return Stack;
})();

js_console_log('Stack.js ok' );
