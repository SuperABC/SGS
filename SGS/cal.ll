@printNum.constStr = constant [3 x i8] c"%d\00", align 1
@printStr.constStr = constant[3 x i8] c"%s\00", align 1
@printFloat.constStr = constant[3 x i8] c"%f\00", align 1
declare i32 @printf(i8*, ...)
declare i32 @putchar(i32)
declare i32 @getchar()
declare i8* @strcpy(i8*, i8*)

define i8 @intToChar(i32) {
	%2 = trunc i32 %0 to i8
	ret i8 %2
}

define i32 @charToInt(i8) {
	%2 = sext i8 %0 to i32
	ret i32 %2
}

define i1 @intToBool(i32) {
	%2 = icmp eq i32 %0, 0
	ret i1 %2
}

define i32 @boolToInt(i1) {
	%2 = sext i1 %0 to i32
	ret i32 %2
}

define float @intToFloat(i32) {
	%2 = sitofp i32 %0 to float
	ret float %2
}

define i32 @floatToInt(float) {
	%2 = fptosi float %0 to i32
	ret i32 %2
}

define i32 @printNum(i32) {
	%2 = call i32(i8*, ...) @printf(i8* getelementptr inbounds([3 x i8], [3 x i8] * @printNum.constStr, i32 0, i32 0), i32 %0)
	ret i32 0
}
define i32 @printFloat(float) {
	%2 = fpext float %0 to double
	%3 = call i32(i8*, ...) @printf(i8* getelementptr inbounds([3 x i8], [3 x i8] * @printFloat.constStr, i32 0, i32 0), double %2)
	ret i32 0
}

define i32 @printStr(i8*) {
	%2 = call i32(i8*, ...) @printf(i8* getelementptr inbounds([3 x i8], [3 x i8] * @printStr.constStr, i32 0, i32 0), i8* %0)
	ret i32 0
}

define i32 @newline() {
	call i32 @putchar(i32 10)
	ret i32 0
}
; ModuleID = 'wtf'
source_filename = "wtf"

@str.array = common global [100 x i32] zeroinitializer
@str = internal global i32* getelementptr inbounds ([100 x i32], [100 x i32]* @str.array, i32 0, i32 0)
@length = common global i32 0
@position = common global i32 0

define i32 @readNum() {
entry:
  %c = alloca i32
  %result = alloca i32
  store i32 0, i32* %result
  %ret.load = load i32, i32* %result
  ret i32 %ret.load
  %call.res = call i32 @getchar()
  store i32 %call.res, i32* %c
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %load.val = load i32, i32* %c
  %lt.res = icmp slt i32 %load.val, 48
  %load.val1 = load i32, i32* %c
  %gt.res = icmp sgt i32 %load.val1, 57
  %or.res = or i1 %lt.res, %gt.res
  br i1 %or.res, label %while.body, label %while.merge

while.body:                                       ; preds = %while.cond
  %call.res2 = call i32 @getchar()
  store i32 %call.res2, i32* %c
  br label %while.cond

while.merge:                                      ; preds = %while.cond
  br label %while.cond3

while.cond3:                                      ; preds = %while.body10, %while.merge
  %load.val4 = load i32, i32* %c
  %lt.res5 = icmp slt i32 %load.val4, 48
  %not.res = icmp eq i1 %lt.res5, false
  %load.val6 = load i32, i32* %c
  %gt.res7 = icmp sgt i32 %load.val6, 57
  %not.res8 = icmp eq i1 %gt.res7, false
  %or.res9 = or i1 %not.res, %not.res8
  br i1 %or.res9, label %while.body10, label %while.merge14

while.body10:                                     ; preds = %while.cond3
  %load.val11 = load i32, i32* %result
  %mul.res = mul i32 %load.val11, 10
  %load.val12 = load i32, i32* %c
  %add.res = add i32 %mul.res, %load.val12
  %sub.res = sub i32 %add.res, 48
  store i32 %sub.res, i32* %result
  %call.res13 = call i32 @getchar()
  store i32 %call.res13, i32* %c
  br label %while.cond3

while.merge14:                                    ; preds = %while.cond3
  %ret.load15 = load i32, i32* %result
  ret i32 %ret.load15
}

define i32 @current() {
entry:
  %result = alloca i32
  store i32 0, i32* %result
  %load.val = load i32, i32* @position
  %load.val1 = load i32, i32* @length
  %gt.res = icmp sgt i32 %load.val, %load.val1
  %not.res = icmp eq i1 %gt.res, false
  br i1 %not.res, label %if.take, label %if.fail

if.take:                                          ; preds = %entry
  %load = load i32, i32* @position
  %array.load = load i32*, i32** @str
  %visit = getelementptr inbounds i32, i32* %array.load, i32 %load
  %load.val2 = load i32, i32* %visit
  store i32 %load.val2, i32* %result
  %ret.load = load i32, i32* %result
  ret i32 %ret.load
  br label %if.merge

if.fail:                                          ; preds = %entry
  br label %if.merge

if.merge:                                         ; preds = %if.fail, %if.take
  store i32 0, i32* %result
  %ret.load3 = load i32, i32* %result
  ret i32 %ret.load3
}

define i32 @match() {
entry:
  %result = alloca i32
  store i32 0, i32* %result
  %load.val = load i32, i32* @position
  %add.res = add i32 %load.val, 1
  store i32 %add.res, i32* @position
  %ret.load = load i32, i32* %result
  ret i32 %ret.load
}

define i32 @parseNum() {
entry:
  %c = alloca i32
  %result = alloca i32
  store i32 0, i32* %result
  %call.res = call i32 @current()
  store i32 %call.res, i32* %c
  store i32 0, i32* %result
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %load.val = load i32, i32* %c
  %lt.res = icmp slt i32 %load.val, 48
  %not.res = icmp eq i1 %lt.res, false
  %load.val1 = load i32, i32* %c
  %gt.res = icmp sgt i32 %load.val1, 57
  %not.res2 = icmp eq i1 %gt.res, false
  %and.res = and i1 %not.res, %not.res2
  br i1 %and.res, label %while.body, label %while.merge

while.body:                                       ; preds = %while.cond
  %load.val3 = load i32, i32* %result
  %mul.res = mul i32 %load.val3, 10
  %load.val4 = load i32, i32* %c
  %add.res = add i32 %mul.res, %load.val4
  %sub.res = sub i32 %add.res, 48
  store i32 %sub.res, i32* %result
  %call.res5 = call i32 @match()
  %call.res6 = call i32 @current()
  store i32 %call.res6, i32* %c
  br label %while.cond

while.merge:                                      ; preds = %while.cond
  %ret.load = load i32, i32* %result
  ret i32 %ret.load
}

define i32 @parseFactor() {
entry:
  %temp = alloca i32
  %c = alloca i32
  %result = alloca i32
  store i32 0, i32* %result
  %ret.load = load i32, i32* %result
  ret i32 %ret.load
  %call.res = call i32 @current()
  store i32 %call.res, i32* %c
  %load.val = load i32, i32* %c
  %lt.res = icmp eq i32 %load.val, 40
  br i1 %lt.res, label %if.take, label %if.fail

if.take:                                          ; preds = %entry
  %call.res1 = call i32 @match()
  %call.res2 = call i32 @parseExpr()
  store i32 %call.res2, i32* %temp
  %call.res3 = call i32 @match()
  %load.val4 = load i32, i32* %temp
  store i32 %load.val4, i32* %result
  %ret.load5 = load i32, i32* %result
  ret i32 %ret.load5
  br label %if.merge

if.fail:                                          ; preds = %entry
  br label %if.merge

if.merge:                                         ; preds = %if.fail, %if.take
  %call.res6 = call i32 @parseNum()
  store i32 %call.res6, i32* %result
  %ret.load7 = load i32, i32* %result
  ret i32 %ret.load7
}

define i32 @parseTerm() {
entry:
  %c = alloca i32
  %result = alloca i32
  store i32 0, i32* %result
  %call.res = call i32 @parseFactor()
  store i32 %call.res, i32* %result
  %call.res1 = call i32 @current()
  store i32 %call.res1, i32* %c
  br label %while.cond

while.cond:                                       ; preds = %if.merge, %entry
  %load.val = load i32, i32* %c
  %lt.res = icmp eq i32 %load.val, 42
  %load.val2 = load i32, i32* %c
  %lt.res3 = icmp eq i32 %load.val2, 47
  %or.res = or i1 %lt.res, %lt.res3
  br i1 %or.res, label %while.body, label %while.merge

while.body:                                       ; preds = %while.cond
  %load.val4 = load i32, i32* %c
  %lt.res5 = icmp eq i32 %load.val4, 42
  br i1 %lt.res5, label %if.take, label %if.fail

if.take:                                          ; preds = %while.body
  %call.res6 = call i32 @match()
  %call.res7 = call i32 @parseFactor()
  %load.val8 = load i32, i32* %result
  %mul.res = mul i32 %load.val8, %call.res7
  store i32 %mul.res, i32* %result
  br label %if.merge

if.fail:                                          ; preds = %while.body
  %call.res9 = call i32 @match()
  %call.res10 = call i32 @parseFactor()
  %load.val11 = load i32, i32* %result
  %div.res = sdiv exact i32 %load.val11, %call.res10
  store i32 %div.res, i32* %result
  br label %if.merge

if.merge:                                         ; preds = %if.fail, %if.take
  %call.res12 = call i32 @current()
  store i32 %call.res12, i32* %c
  br label %while.cond

while.merge:                                      ; preds = %while.cond
  %ret.load = load i32, i32* %result
  ret i32 %ret.load
}

define i32 @parseExpr() {
entry:
  %c = alloca i32
  %result = alloca i32
  store i32 0, i32* %result
  %call.res = call i32 @parseTerm()
  store i32 %call.res, i32* %result
  %call.res1 = call i32 @current()
  store i32 %call.res1, i32* %c
  br label %while.cond

while.cond:                                       ; preds = %if.merge, %entry
  %load.val = load i32, i32* %c
  %lt.res = icmp eq i32 %load.val, 43
  %load.val2 = load i32, i32* %c
  %lt.res3 = icmp eq i32 %load.val2, 45
  %or.res = or i1 %lt.res, %lt.res3
  br i1 %or.res, label %while.body, label %while.merge

while.body:                                       ; preds = %while.cond
  %load.val4 = load i32, i32* %c
  %lt.res5 = icmp eq i32 %load.val4, 43
  br i1 %lt.res5, label %if.take, label %if.fail

if.take:                                          ; preds = %while.body
  %call.res6 = call i32 @match()
  %call.res7 = call i32 @parseTerm()
  %load.val8 = load i32, i32* %result
  %add.res = add i32 %load.val8, %call.res7
  store i32 %add.res, i32* %result
  br label %if.merge

if.fail:                                          ; preds = %while.body
  %call.res9 = call i32 @match()
  %call.res10 = call i32 @parseTerm()
  %load.val11 = load i32, i32* %result
  %sub.res = sub i32 %load.val11, %call.res10
  store i32 %sub.res, i32* %result
  br label %if.merge

if.merge:                                         ; preds = %if.fail, %if.take
  %call.res12 = call i32 @current()
  store i32 %call.res12, i32* %c
  br label %while.cond

while.merge:                                      ; preds = %while.cond
  %ret.load = load i32, i32* %result
  ret i32 %ret.load
}

define i32 @main() {
entry:
  %res = alloca i32
  %c = alloca i32
  br label %while.cond

while.cond:                                       ; preds = %while.merge, %entry
  br i1 true, label %while.body, label %while.merge12

while.body:                                       ; preds = %while.cond
  store i32 0, i32* @length
  store i32 0, i32* @position
  %call.res = call i32 @getchar()
  store i32 %call.res, i32* %c
  br label %while.cond1

while.cond1:                                      ; preds = %if.merge, %if.take, %while.body
  %load.val = load i32, i32* %c
  %lt.res = icmp eq i32 %load.val, 10
  br i1 %lt.res, label %while.body2, label %while.merge

while.body2:                                      ; preds = %while.cond1
  %load.val3 = load i32, i32* %c
  %lt.res4 = icmp eq i32 %load.val3, 32
  br i1 %lt.res4, label %if.take, label %if.fail

if.take:                                          ; preds = %while.body2
  %call.res5 = call i32 @getchar()
  store i32 %call.res5, i32* %c
  br label %while.cond1
  br label %if.merge

if.fail:                                          ; preds = %while.body2
  br label %if.merge

if.merge:                                         ; preds = %if.fail, %if.take
  %load = load i32, i32* @length
  %array.load = load i32*, i32** @str
  %visit = getelementptr inbounds i32, i32* %array.load, i32 %load
  %load.val6 = load i32, i32* %c
  store i32 %load.val6, i32* %visit
  %load.val7 = load i32, i32* @length
  %add.res = add i32 %load.val7, 1
  store i32 %add.res, i32* @length
  %call.res8 = call i32 @getchar()
  store i32 %call.res8, i32* %c
  br label %while.cond1

while.merge:                                      ; preds = %while.cond1
  %call.res9 = call i32 @parseNum()
  store i32 %call.res9, i32* %res
  %load10 = load i32, i32* %res
  %call.res11 = call i32 @printNum(i32 %load10)
  br label %while.cond

while.merge12:                                    ; preds = %while.cond
  ret i32 0
}
