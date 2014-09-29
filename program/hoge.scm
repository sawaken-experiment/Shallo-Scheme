(load 'macro.scm)

;(define a 3)

;(define f (lambda (n) (if (= n 0) 1 (* n (f (- n 1))))))

(define acc (lambda (n a)
	      (let ((hoge 1))
		(if (= n 0) a (acc (- n 1) (+ a 1))))))

(define acc-not-tail (lambda (n) (if (= n 0) 0 (+ 1 (acc-not-tail (- n 1))))))

(define var-len-f (lambda (a b . c) c))

(define count-down (lambda (n) (display n) (if (= n 0) n (count-down (- n 1)))))

(load 'loaded.scm)

(+ *in-loaded* 1)

(var-len-f 1 2)

(eval '(+ 1 3) (interaction-environment))


(acc 1 0)

;(count-down 10)

;((lambda (a b) a b) 1 2)

;(let ((a 1) (b 2)) (display (- a b)) (+ a b))

;(+ (catch (+ 1 (+ 1 (raise 1)))) 1)

(catch (lambda (exc) (display exc))
       ((lambda () (define x 1) (display x) (+ x x))))


(define closure ((lambda (x) (lambda (y) (+ x y))) 10))
(display (closure 5))

(define *count* 10)
(define *cont* ())

(display (+ 1 (call/cc (lambda (c) (set! *cont* c) 1))))
(set! *count* (- *count* 1))
(if (= *count* 0) 'end (*cont* 1))

(+ (splicing '(10 100)))
(splicing '(1 2))




(define lst '(4 5 6))
(define getlst (lambda () lst))
(define f (lambda (n) (* n n)))
(define getf (lambda () f))
(define lst2 (map f lst))

`(1 2 ,(+ 1 2) ,@(map f lst))

