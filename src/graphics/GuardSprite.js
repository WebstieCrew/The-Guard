export class ShieldState {
  static IDLE = 'IDLE';
  static SCANNING = 'SCANNING';
  static THREAT_DETECTED = 'THREAT_DETECTED';
  static SECURED = 'SECURED';
}

export class GuardSprite {
  constructor() {
    this.position = { x: 0, y: 0 };
    this.scale = { x: 1, y: 1 };
    this.rotation = 0;
    this.opacity = 1;
    this.currentState = ShieldState.IDLE;
    this.texture = null;
  }

  async loadTexture(src) {
    return new Promise((resolve, reject) => {
      const img = new Image();
      img.onload = () => {
        this.texture = img;
        resolve(true);
      };
      img.onerror = () => reject(new Error(`Failed to load texture: ${src}`));
      img.src = src;
    });
  }

  setPosition(x, y) {
    this.position.x = x;
    this.position.y = y;
  }

  setScale(x, y) {
    this.scale.x = x;
    this.scale.y = y;
  }

  setRotation(angle) {
    this.rotation = angle;
  }

  setState(state) {
    this.currentState = state;
  }

  update(deltaTime) {
    switch (this.currentState) {
      case ShieldState.SCANNING:
        this.rotation = (this.rotation + 90 * deltaTime) % 360;
        break;
      case ShieldState.THREAT_DETECTED:
        this.scale.x = 1 + 0.05 * Math.sin(this.rotation);
        this.scale.y = this.scale.x;
        this.rotation = (this.rotation + 10 * deltaTime) % 360;
        break;
      case ShieldState.SECURED:
        this.scale = { x: 1, y: 1 };
        this.rotation = 0;
        break;
      case ShieldState.IDLE:
      default:
        break;
    }
  }

  render(ctx) {
    if (!ctx || !this.texture) return;

    ctx.save();
    ctx.translate(this.position.x, this.position.y);
    ctx.rotate((this.rotation * Math.PI) / 180);
    ctx.scale(this.scale.x, this.scale.y);
    ctx.globalAlpha = this.opacity;

    ctx.drawImage(
      this.texture,
      -this.texture.width / 2,
      -this.texture.height / 2
    );

    ctx.restore();
  }
}
