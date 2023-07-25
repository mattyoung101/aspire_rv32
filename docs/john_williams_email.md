## Thesis project proposal on RISC-V CPU design for Semester 2 2024

Hi John,

My name is Matt Young, and I will be graduating the Bachelor of Computer Science program here at UQ next
semester. I'm planning to pursue a Bachelor of Computer Science (Honours) degree after that, which involves
a research thesis.

I am planning to do a thesis on designing a simple 32-bit RISC-V RV32IC CPU, implemented using SystemVerilog, on a
Lattice ECP5 FPGA. I've always wanted to design a CPU since about late 2021, and since then I've been
improving my skills in digital hardware design and particularly in SystemVerilog. If the project is able
to go ahead, it may be the first CPU designed at UQ in some number of years, and possibly the first RISC-V
CPU designed entirely in-house, from scratch, at UQ.

I've attached my proposal as a PDF in this email, it contains all the information about design goals, the
verification plan, some details about the planned microarchitecture and benchmarks that will be performed on
the finished design. Overall, it's a relatively simple design (e.g. no pipelining), which is done to ensure
that I can complete the project in time. However, I've included some extension tasks that I can complete if
time permits, and would create a significantly more powerful microarchitecture.

Another interesting thing about this project is that I am to use entirely open-source tools, such as Verilator
and the Yosys suite, to complete my design. These are the tools I have the most experience in, and so far have
proved very capable in synthesising SV code to the Lattice ECP5 I own. The verification stage is based on the
latest instruction generation/fuzzing techniques employed by large companies, and there's also an interesting
"speculative pre-fetch" stage that I will implement if time permits.

If you have the time available, and are interested in my proposal, your involvement as a supervisor for this
project would be very much appreciated. I think with your experience at Xilinx and porting the Linux kernel to
MicroBlaze, you would be the perfect supervisor and I'd be sure to learn a lot.

Thanks for your consideration,
Matt.

