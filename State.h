class StateContext;

struct InputData {
  long a1_min_, a1_max_;
  long a2_min_, a2_max_;
  long a1_range_, a2_range_;
  long A1, A2, P1, P2;
  long hold_;
  bool use_filter_;
  long last_a1_;

  void Init(long a1_min, long a1_max, long a2_min, long a2_max, bool use_filter) {
    a1_min_ = a1_min;
    a1_max_ = a1_max;
    a2_min_ = a2_min;
    a2_max_ = a2_max;
    a1_range_ = a1_max - a1_min;
    a2_range_ = a2_max - a2_min;
    last_a1_ = a1_min;
  }

  long Filter(long value) {
    auto ratio = 0.2;
    last_a1_ = (1 - ratio) * last_a1_ + ratio * value;
    return last_a1_;
  }

  void Update(long a1, long a2) {
    P1 = ((a1 - a1_min_) * 100) / a1_range_;
    P2 = ((a2 - a2_min_) * 100) / a2_range_;
    A1 = Filter(a1);
    A2 = a2;
  }
};

struct OutData {
  long A1;
  long A2;
};

class State {
public:
  virtual void Update(StateContext* state_context, InputData* in, OutData* out) = 0;
};

class StateNone: public State {
public:
  StateNone() { Serial.println("StateNone"); }
  void Update(StateContext* state_context, InputData* in, OutData* out);
};

class BitePointMode: public State {
public:
  BitePointMode() { Serial.println("BitePointMode"); }
  void Update(StateContext* state_context, InputData* in, OutData* out);
};

class LaunchMode: public State {
public:
  LaunchMode() { Serial.println("LaunchMode"); }
  void Update(StateContext* state_context, InputData* in, OutData* out);
};

class StateContext {
  private:
    State* state_;
  
  public:
    StateContext(void):state_(new StateNone()) {    
    }

    void SetState(State *state) {
      if(state_) delete state_;
      state_ = state;
    }

  void Update(InputData* in, OutData* out) {
    state_->Update(this, in, out);
  }
};

void StateNone::Update(StateContext* state_context, InputData* in, OutData* out) {
  out->A1 = in->A1;
  if(in->P2 > 90) {
    state_context->SetState(new BitePointMode());
  }
}
void BitePointMode::Update(StateContext* state_context, InputData* in, OutData* out) {
  out->A1 = in->a1_max_;
  if(in->P2 < 5) {
    state_context->SetState(new LaunchMode());
  }
}
void LaunchMode::Update(StateContext* state_context, InputData* in, OutData* out) {
  out->A1 = max(in->hold_, in->A1);
  if(in->P1 < 5) {
    state_context->SetState(new StateNone());
  }
}
