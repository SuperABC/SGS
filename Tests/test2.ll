
@printNum.constStr = constant [3 x i8] c"%d\00", align 1
@printStr.constStr = constant[3 x i8] c"%s\00", align 1
@printFloat.constStr = constant[3 x i8] c"%f\00", align 1
declare i32 @printf(i8*, ...)
declare i32 @scanf(i8*, ...)
declare i32 @putchar(i32)
declare i32 @getchar()
declare i8* @strcpy(i8*, i8*)

define i8 @intToChar(i32) {
	%2 = trunc i32 %0 to i8
	ret i8 %2
}

define i32 @charToInt(i8) {
	%2 = alloca i8, align 1
	store i8 %0, i8* %2, align 1
	%3 = load i8, i8* %2, align 1
	%4 = sext i8 %3 to i32
	ret i32 %4
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

define i32 @readNum() {
	%1 = alloca i32, align 4
	%2 = call i32(i8*, ...) @scanf(i8* getelementptr inbounds([3 x i8], [3 x i8] * @printNum.constStr, i32 0, i32 0), i32* %1)
	%3 = load i32, i32* %1, align 4
	ret i32 %3
}
define i32 @newline() {
	call i32 @putchar(i32 10)
	ret i32 0
}

define i32 @readStr(i8*)  {
	%2 = call i32(i8*, ...) @scanf(i8* getelementptr inbounds([3 x i8], [3 x i8] * @printStr.constStr, i32 0, i32 0), i8* %0)
	ret i32 0
}
; ModuleID = 'wtf'
source_filename = "wtf"

@i = common global i32 0
@fact = common global float 0.000000e+00
@res = common global float 0.000000e+00

define i32 @main() {
entry:
  store i32 1, i32* @i
  store float 1.000000e+00, float* @fact
  store float 0.000000e+00, float* @res
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %load.val = load i32, i32* @i
  %lt.res = icmp slt i32 %load.val, 11
  br i1 %lt.res, label %while.body, label %while.merge

while.body:                                       ; preds = %while.cond
  %load.val1 = load float, float* @fact
  %fdiv.res = fdiv float 1.000000e+00, %load.val1
  %load.val2 = load float, float* @res
  %fadd.res = fadd float %load.val2, %fdiv.res
  store float %fadd.res, float* @res
  %load = load i32, i32* @i
  %call.res = call float @intToFloat(i32 %load)
  %load.val3 = load float, float* @fact
  %fmul.res = fmul float %load.val3, %call.res
  store float %fmul.res, float* @fact
  %load.val4 = load i32, i32* @i
  %add.res = add i32 %load.val4, 1
  store i32 %add.res, i32* @i
  br label %while.cond

while.merge:                                      ; preds = %while.cond
  %load5 = load float, float* @res
  %call.res6 = call i32 @printFloat(float %load5)
  ret i32 0
}
