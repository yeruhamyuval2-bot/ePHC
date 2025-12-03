### 4 — “**Why Amazon (Annapurna Labs)?**” — *Leadership-Principles edition* (≈ 45 s)

1. **Customer Obsession → Think Big**
   I’m driven by unseen impact: the Wi-Fi + GNSS firmware I wrote at Sony now sits in millions of devices. Annapurna’s chips power *every* AWS customer workload, so optimizing a few microseconds or milliwatts here scales to planet-size benefit.

2. **Ownership → Deliver Results**
   From first silicon power-on to production sign-off, I owned full bring-up of a new Wi-Fi MAC on CEVA cores—debugging in Palladium, hitting perf targets, and staying on call until the last bug closed. I’ll bring that same end-to-end accountability to Nitro & Graviton firmware blocks.

3. **Invent and Simplify**
   I slashed validation time 40 % by dockerizing toolchains and auto-generating Python test suites. I love turning messy, manual flows into one-click pipelines—exactly the mindset Annapurna uses to erase complexity for AWS builders.

4. **Dive Deep → Are Right, A Lot**
   Whether tracing RF glitches with a logic analyzer or stepping through ISR timing, I back decisions with data. That rigor is vital when your firmware must squeeze nanoseconds out of PCIe paths or meet cloud-scale reliability goals.

5. **Learn and Be Curious → Hire & Develop the Best**
   Annapurna’s Tel-Aviv/Haifa teams sit at the crossroads of silicon, firmware, and cloud architecture—the perfect classroom. I’m eager to absorb that knowledge and, in turn, mentor interns and new grads in Hebrew and English.

*In short, Annapurna’s culture maps perfectly to how I already work—own the problem, invent boldly, sweat the details, and never stop learning—so I can help push AWS silicon to the next summit.*

## Curiousity questions

Customer Obsession: “Walk me through a time you uncovered a latent customer need while working deep in the stack. How did you validate it and translate it into silicon- or firmware-level changes?” - Do you treat internal platform teams or EC2 tenants as “customers,” gather data, and close the feedback loop?

Success and Scale Bring Broad Responsibility: “Firmware changes you ship affect millions of workloads. How do you evaluate environmental or societal impact before you merge?” - Big-picture ethics and sustainability mindset.

**Amazon’s 16 Leadership Principles (2025)**

1. Customer Obsession
2. Ownership
3. Invent and Simplify
4. Are Right, A Lot
5. Learn and Be Curious
6. Hire and Develop the Best
7. Insist on the Highest Standards
8. Think Big
9. Bias for Action
10. Frugality
11. Earn Trust
12. Dive Deep
13. Have Backbone; Disagree and Commit
14. Deliver Results
15. **Strive to be Earth’s Best Employer**
16. **Success and Scale Bring Broad Responsibility** ([amazon.jobs][1])

---

### Key Annapurna Labs Hardware Platforms

| Platform                            | What it is                                                                                                                                     | Why it matters                                                                                                                                                                                                                                 |
| ----------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **AWS Nitro System**                | A family of off-load cards, a security chip, and a lightweight hypervisor that move storage, network, and management functions out of the CPU. | Frees nearly all host compute/memory for customer VMs, hardens security (no admin shell), and enables bare-metal EC2 while cutting cost—core to every modern EC2 instance. ([Amazon Web Services, Inc.][2])                                    |
| **AWS Graviton (Arm CPUs)**         | Custom Arm-based server processors (Graviton → Graviton2 → Graviton3 → **Graviton4, 2024**).                                                   | Up to 60 % higher compute performance and best-in-class perf/W on memory-optimized X8g and C8gn instances; drives price-performance leadership for general workloads. ([Amazon Web Services, Inc.][3])                                         |
| **AWS Inferentia (Inference ASIC)** | Deep-learning inference accelerators (Inferentia & **Inferentia2**) built around NeuronCores.                                                  | Inferentia2 delivers **4×** the throughput and **10×** lower latency vs. the first gen, with 32 GB HBM per chip and NeuronLink interconnect—powering low-cost, high-speed Gen-AI inference on Inf2 instances. ([Amazon Web Services, Inc.][4]) |
| **AWS Trainium (Training ASIC)**    | Training-focused AI accelerators (Trainium & **Trainium2**).                                                                                   | Trainium2 offers **4×** the performance of Trainium, 1.5 TB HBM per Trn2 instance (or 6 TB in UltraServers), and up to 83 PFLOPS FP8—slashing model-training cost 30–40 % vs. top GPUs. ([Amazon Web Services, Inc.][5])                       |

**How they fit together**

* **Nitro** provides the secure, performant substrate for every EC2 server—its off-load cards and security chip are literally present on each Graviton, Trainium, or Inferentia host.
* **Graviton** CPUs run general-purpose compute; teams compile typical ARM64 Linux apps or container workloads for exceptional price-performance.
* **Inferentia2** plugs into Nitro-based Inf2 instances to serve real-time Gen-AI and LLM inference at low latency.
* **Trainium2** powers Trn2 instances and UltraServers for massive-scale model training; developers can prototype on smaller Trn1 boxes and scale out seamlessly with NeuronLink.

Together these Annapurna-designed chips let AWS own the entire silicon stack—CPU, networking, storage off-load, inference, and training—mirroring Amazon’s leadership-principle focus on **Invent and Simplify**, **Customer Obsession**, and **Frugality** while enabling cloud customers to run faster, cheaper, and more securely.

[1]: https://www.amazon.jobs/content/our-workplace/leadership-principles "Leadership Principles"
[2]: https://aws.amazon.com/ec2/nitro/ "AWS Nitro System"
[3]: https://aws.amazon.com/blogs/aws/now-available-graviton4-powered-memory-optimized-amazon-ec2-x8g-instances/ "Now available: Graviton4-powered memory-optimized Amazon EC2 X8g instances | AWS News Blog"
[4]: https://aws.amazon.com/ai/machine-learning/inferentia/ "AI Chip - Amazon Inferentia - AWS"
[5]: https://aws.amazon.com/ai/machine-learning/trainium/ "AI Accelerator - AWS Trainium - AWS"
