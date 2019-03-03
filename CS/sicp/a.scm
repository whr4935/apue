(define (sqrt2 x)
  (sqrt-iter 1.0 x))

(define (average x y)
  (/ (+ x y) 2))

(define (square x)
  (* x x))

(define (good-enough? guess x)
  (< (abs (- (square guess) x)) 0.001))

(define (improve guess x)
  (average guess (/ x guess)))

(define (sqrt-iter guess x)
  (if (good-enough? guess x)
      guess
      (sqrt-iter (improve guess x) x)))


;; (sqrt2 100)
;; (sqrt 100)
;; (begin (display "hello whr")(newline))


;; factorial
(define (factorial n)
  (if (= n 1)
      1
      (* n (factorial(- n 1)))))

;; (factorial 5)

(define (f x y)
  ((lambda (a b)
    (+ (* x a)
       (* y b)
       (* a b)))
   (+ 1 (* x y))
   (- 1 y)))

;; (f 1 1)

(+ (let (x 3) (+ x (* x 10))) 0)