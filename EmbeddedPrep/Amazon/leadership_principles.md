
## Bias for Action

### First question

> “Tell me about a time when you worked against tight deadlines and didn’t have time to consider all options before making a decision. What was the situation? What approach did you take? What did you learn?”

**S – Situation:**
After months of Wi-Fi bring-up, we were facing aggressive deadlines to finalize RF arbitration for multi-radio coexistence. Just after integrating this feature, we started seeing devices freeze during runtime.

**T – Task:**
We had to identify and resolve a critical issue — an apparent infinite loop of hardware interrupts — under tight time pressure. Delaying the fix would risk the entire project schedule and integration with other teams.

**A – Action:**
Given the lack of deep debug tools and time constraints, we couldn’t afford to perform full root-cause analysis. Instead, we rapidly collected runtime logs and reproduced the failure in minimal test cases. I proposed a workaround: temporarily disable the interrupt handler when another radio technology was active, then safely shut down the Wi-Fi scan flow. This avoided entering the stuck state without modifying the core arbitration logic.

**R – Result:**
The workaround fully resolved the freeze. It allowed us to proceed with full feature integration and release validation. The same workaround later became the foundation for the final fix after deeper investigation. It unblocked a major milestone and helped close a long-standing issue.

## Deep Dive

### First question

>“Tell me about a time when you were trying to understand a complex problem on your team and had to dig into the details to figure it out.”

**S – Situation:**
When I transitioned into a new team working on GNSS firmware, I was tasked with evaluating a new Assisted GNSS feature that another team proposed for integration into our product.

**T – Task:**
I needed to assess whether this feature could be supported on our embedded platform — both technically and architecturally — and identify any risks it posed to our existing flows.

**A – Action:**
Since I wasn’t yet familiar with Assisted GNSS, I had to ramp up quickly. I used a combination of technical documentation, online research, and GenAI-based tools to understand the protocols, assistance flows, and timing models.
After analyzing our current architecture and constraints, I discovered multiple potential issues — such as increased memory usage during cold starts and potential race conditions in our acquisition logic. I mapped the expected integration points and analyzed how they would interact with our L1-only firmware.

**R – Result:**
I concluded that while Assisted GNSS offered clear benefits for acquisition time, integrating it in our current stack would risk breaking timing guarantees and destabilizing flow control. I presented this analysis to my lead and proposed a phased approach: isolate Assisted GNSS support behind compile-time flags and introduce it only when we add a compatible scheduling layer. This was accepted, and it became the basis for our Assisted GNSS roadmap.

### Second question

> Tell me about a situation that required you to dig deep to get to the root cause. How did you know you were focusing on the right things?

**S – Situation:**
During Wi-Fi bring-up on a new platform, we were unable to receive any beacon frames — a basic requirement to validate our receiver path. This issue blocked all further testing and progress.

**T – Task:**
Our goal was to identify why beacon reception was failing at the firmware/hardware boundary. The system appeared to be configured correctly, and we had already verified register setups and clocks.

**A – Action:**
Initially, we focused on verifying high-level configurations: register maps, RF clocks, and interrupt lines. But none of these revealed the issue.
At that point, I proposed we **rebuild our bring-up path from the ground up**, revalidating low-level assumptions — even things we thought were already working.
We systematically tested the memory-mapped regions and discovered that one memory access — assumed to be stable — was silently failing. A deeper code review showed that a critical pointer calculation was incorrect, causing writes to go to an invalid region of hardware memory.

**R – Result:**
After correcting the memory access logic, the device was immediately able to receive beacons, unblocking the rest of the MAC bring-up flow.
This incident emphasized the need to validate even foundational assumptions and inspired us to build a structured bring-up checklist to avoid similar blind spots in future projects.

## Are Right, A lot

### First Question

> “Tell me about a time when you didn’t have enough data to make the right decision. What did you do? What path did you take? Did the decision turn out to be the correct one?”

**S – Situation:**
For a new chip launch, I was responsible for designing a C-based API layer for our LwM2M stack. Until then, our system used AT commands to expose functionality, but the new chip demanded more modern and structured access for embedded developers.

**T – Task:**
I had to choose the architectural path for exposing these APIs: either reuse the AT-based flow or adopt a more advanced ERPC (Embedded Remote Procedure Call) mechanism. The decision needed to be made quickly to unblock development, and I didn’t have time to fully prototype both options or validate their long-term implications.

**A – Action:**
I evaluated both paths based on developer ergonomics, code maintainability, and feature flexibility. While the AT path was simpler and already integrated, it had limitations in modularity and input/output complexity.
I chose to implement the ERPC-based architecture. I knew it would be harder initially, but I believed it would unlock better future extensibility. I created the foundational ERPC infrastructure, documented the APIs, and supported onboarding of other teams.

**R – Result:**
The ERPC APIs worked well from a feature and structure perspective. However, in the long term, we faced maintainability issues — we now had to support both the legacy AT interface and the new ERPC stack, essentially doubling the validation effort.
Looking back, I would have scoped a minimum-viable AT extension first, then evaluated ERPC in a second phase once the product was stable. This experience taught me to explicitly factor *operational cost* into early design decisions — not just architectural purity.

## Ownership

> “Tell me about a time when you took on something significant outside your area of responsibility. Why was it important? What was the outcome?”

**S – Situation:**
During the early bring-up of a new Wi-Fi subsystem, I was paired with another engineer — I was responsible for the RF path, while she was assigned the MAC and application layer integration.

**T – Task:**
After completing the RF bring-up, I saw that the MAC/application integration was facing issues that were blocking end-to-end testing. Although it wasn’t my area of responsibility, progress had stalled and deadlines were approaching.

**A – Action:**
I stepped in to support the MAC flow analysis. I helped debug the control sequences, identify protocol mismatches, and troubleshoot the scan state machine. I also took ownership of more advanced bring-up environments like Palladium simulation, which required complex setup and vendor coordination.
Over time, I gained deep familiarity with both RF and MAC sides — and began supporting other teams (like QA and integration) whenever Wi-Fi issues arose. I documented critical flows, helped onboard new team members, and acted as the go-to person for any Wi-Fi-related problem.

**R – Result:**
The Wi-Fi bring-up was completed successfully and on schedule. I became the main point of contact for Wi-Fi firmware across the company — effectively acting as a gatekeeper for all technical flows involving the subsystem. This early ownership helped stabilize the product and enabled parallel teams to integrate with confidence.

## Deliver Results

### First question

>“Give me an example of a time when you were able to deliver an important project under a tight deadline. What sacrifices did you have to make to meet the deadline? How did they impact the final deliverable?”

**S – Situation:**
After two of our managers returned from the Sony HQ in Japan, our team was tasked with supporting a high-stakes demo app — showcasing all major chip features — to be completed in just two weeks. The request came unexpectedly, and timelines were extremely tight.

**T – Task:**
I volunteered to take full ownership of power optimization. This meant I had to quickly understand the full system behavior, identify energy drains across subsystems, and implement improvements without compromising functionality.

**A – Action:**
Just days into the effort, a missile attack from Iran impacted our region as part of the ongoing war. Despite the escalating situation, I remained committed to delivering the demo.
While working under sirens and high stress, I made the decision to drive into the empty office — not only to gain access to better debugging equipment, but also to enable remote work continuity for my team.
Through focused analysis, I identified key inefficiencies and implemented low-power modes and scheduling improvements. I also coordinated with RF and sensor teams to gate activity where possible.

**R – Result:**
We successfully delivered the demo on time. My optimizations resulted in over **75% power savings**, making the demo far more effective and customer-ready.
Beyond the technical achievement, this effort demonstrated our team’s resilience — and reinforced that even in the midst of war, we could uphold our professional standards and deliver with integrity.

## Invent and Simplify

### First question

> “Give me an example of a complex problem you solved with a simple solution. What made the problem complex? How do you know your solution addressed the problem?”

**S – Situation:**
Our company’s main testing framework was a proprietary GUI-based tool. It was difficult to use, poorly scalable, and a poor fit for modern automated development practices. It created friction for R\&D engineers who needed more flexibility.

**T – Task:**
I wanted to enable R\&D teams to write scalable, scriptable tests — with the goal of increasing test coverage, improving maintainability, and potentially integrating AI into test authoring.

**A – Action:**
I already had a Python-based simulator that could interface with the device under test. I chose an open-source Python testing library and adapted it to work with our devices. I built the necessary infrastructure to abstract low-level control and created helper interfaces so teams could write test cases in Python instead of manually clicking through the GUI.
I also designed the architecture to be extensible — allowing AI tools to help generate test cases and automate edge case discovery in the future.

**R – Result:**
The new framework empowered multiple R\&D teams to write tests in code, enabled automation pipelines, and significantly reduced validation cycle time. It also laid the groundwork for AI-based test authoring and coverage analysis.
This replaced a brittle, manual workflow with a modern, flexible system — improving velocity and team satisfaction.

## Earn trust

### First question

> “Tell me about a time when you uncovered a significant problem in your team. How did you communicate it? How did you manage the impact?”

**S – Situation:**
When I joined the GNSS team after completing the Wi-Fi bring-up, I inherited a codebase that lacked clear separation of layers, consistent API design, and proper protection against misuse. I knew that if we didn’t address these issues early, it would become very difficult to evolve the APIs once released to external teams.

**T – Task:**
My goal was to restructure the firmware APIs — introducing proper error handling, layered architecture, and interfaces that were safe and future-proof. But I also knew that changing these foundations could destabilize the system and trigger concerns from the team.

**A – Action:**
I proposed the changes transparently and acknowledged the risk. To build trust, I used my Python test framework to systematically validate every change. I worked after hours to ensure full test coverage and minimize regressions, and I proactively communicated progress in code reviews and team syncs.
Looking back, I realize I introduced multiple changes at once. I should have broken them into smaller, safer PRs to ease review and isolate risk. Still, I made sure the team could trust that stability was preserved.

**R – Result:**
The changes were accepted, and the improved API structure helped us scale the product with fewer integration issues. More importantly, I built credibility with the team — they knew I took quality seriously and followed through.
The experience taught me how to **earn technical trust** by coupling bold ideas with transparent communication, test rigor, and continuous feedback.

Tehnical things:

- Sperate messages flows to decrese uninon typs messages
- Sync all APIs with thread safe semaphores
- Seperate layers for better encapsulation

## Have Backbone; Disagree and Commit

>“Tell me about a time when you strongly disagreed with your manager or peer on something important to the business. What was it and how did you handle it?”

**S – Situation:**
In my GNSS team, a senior developer preferred a coding style where modules accessed each other through global includes and shared headers, bypassing structured API boundaries. I believed this approach risked breaking abstraction and maintainability.

**T – Task:**
I advocated for encapsulating logic under clearly defined APIs to preserve modularity and enable long-term scalability. However, the developer insisted his approach was sufficient and more efficient for the current phase of development.

**A – Action:**
I respectfully raised the concern during code reviews and one-on-one discussions, pointing out specific risks like coupling, unit testability, and interface fragility. When we couldn’t align, I escalated to our manager, who supported my view — but we also agreed to allow some coding flexibility across the team to balance progress and ownership.
Despite disagreeing, I committed to the team direction, documented my concerns, and offered to refactor later if issues emerged.

**R – Result:**
A few months later, several bugs emerged in those modules. We traced them back to logic that bypassed the formal APIs — exactly the scenario I had warned about. This validated the need for stronger encapsulation and led to new team guidelines enforcing API boundaries.
The experience taught me to stand firm when I see systemic risks — but also to respect team alignment once a decision is made, and let the outcomes guide future improvement.

## Think Big

> “Tell me about a time when you were working on an initiative and saw an opportunity to do something much bigger or better than the initial focus. Did you take that opportunity?”

**S – Situation:**
When I joined the GNSS team, the core firmware bring-up was nearly complete and we were moving into the testing phase. I had already developed a basic test driver to validate the firmware behavior.

**T – Task:**
My job was to support validation of the GNSS system. But I realized that simply running tests wasn’t enough — we had no system-wide visibility into performance trends, historical regressions, or analytics to guide debugging and product tuning.

**A – Action:**
I saw an opportunity to turn my test setup into a full observability stack. I integrated modern open-source tools — typically used for cloud analytics — into our embedded validation workflow. I added logging pipelines, performance metrics tracking, and a dashboard to visualize GNSS signal stability, acquisition times, and regression trends over time.
This let us track behavior across test runs, compare performance across builds, and identify anomalies early.

**R – Result:**
The upgraded environment gave us a **productivity boost** during testing and significantly improved our ability to debug complex issues. It became the go-to system for both validation and firmware teams to understand real-world GNSS behavior.
By thinking beyond just “make tests pass,” I helped the team adopt a **data-driven development mindset** and built infrastructure that scaled with the product.
