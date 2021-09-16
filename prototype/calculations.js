const HEIGHT = 500;
const WIDTH = 500;

const SOLAR_PANEL_WIDTH = 45;
const SOLAR_PANEL_LENGTH = 65;

// The inner ring circumference is 408"
const INNER_CIRCUMFERENCE = 408;
const INNER_RADIUS = INNER_CIRCUMFERENCE / Math.PI / 2; // C=2πr

// The outer ring is 65" wider than the inside ring
const OUTER_RADIUS = INNER_RADIUS + SOLAR_PANEL_LENGTH;

const XY = (x, y) => Object.freeze({ x, y });

const Ring = (radius, origin) => {
  const ring = {
    cx: origin.x,
    cy: origin.y,
    origin,
    r: radius
  };
  return Object.freeze({
    ...ring,
    draw: ctx => {
      ctx.beginPath();
      ctx.arc(ring.cx, ring.cy, ring.r, 0, 2 * Math.PI);
      ctx.stroke();
    }
  });
};

const Dot = xy => Object.freeze({
  x: xy.x,
  y: xy.y,
  draw: ctx => {
    ctx.beginPath();
    ctx.arc(xy.x, xy.y, 2, 0, 2 * Math.PI);
    ctx.fill();
  }
});

const Rectangle = (p1, p2, p3, p4) => Object.freeze({
  p1,
  p2,
  p3,
  p4,
  draw: ctx => {
    ctx.beginPath();
    ctx.moveTo(p1.x, p1.y);
    ctx.lineTo(p2.x, p2.y);
    ctx.lineTo(p3.x, p3.y);
    ctx.lineTo(p4.x, p4.y);
    ctx.fillStyle = 'rgba(0, 255, 255, 0.5)';
    ctx.fill();
  }
});

const Line = (p1, p2) => Object.freeze({
  p1,
  p2,
  draw: ctx => {
    ctx.beginPath();
    ctx.moveTo(p1.x, p1.y);
    ctx.lineTo(p2.x, p2.y);
    ctx.strokeStyle = 'blue';
    ctx.stroke();
  },
  log: [
    '(',
    Math.round(p1.x),
    Math.round(p1.y),
    ') -> (',
    Math.round(p2.x),
    Math.round(p2.y),
    ')'
  ].join(' ')
});

const pointOnCircumference = (radius, origin, d) => {
  const degrees = d - 90 < 0 ? d + 270 : d - 90;
  const radians = (degrees * Math.PI) / 180;
  return XY(
    origin.x + radius * Math.cos(radians),
    origin.y + radius * Math.sin(radians)
  );
};

const inner = Ring(INNER_RADIUS, XY(WIDTH / 2, HEIGHT / 2));
const outer = Ring(OUTER_RADIUS, XY(WIDTH / 2, HEIGHT / 2));

const draw = () => {
  const canvas = document.getElementById('canvas');
  if (canvas.getContext) {
    const ctx = canvas.getContext('2d');

    inner.draw(ctx);
    outer.draw(ctx);

    // Calculate the corners of the solar panels
    const innerPanelCorners = d3.range(0, 360, 40).map(degrees =>
      pointOnCircumference(inner.r, inner.origin, degrees)
    );
    const outerPanelCorners = d3.range(10, 360, 20).map(degrees =>
      pointOnCircumference(outer.r, outer.origin, degrees)
    );
    innerPanelCorners.forEach(xy => Dot(xy).draw(ctx));
    outerPanelCorners.forEach(xy => Dot(xy).draw(ctx));

    // Draw the solar panels as rectangles
    const solarPanels = d3.range(0, 9).map(i => Rectangle(
      innerPanelCorners[i],
      outerPanelCorners[i * 2],
      outerPanelCorners[(i * 2) + 1],
      innerPanelCorners[i + 1 >= 9 ? 0 : i + 1]
    ));
    solarPanels.forEach(panel => panel.draw(ctx));

    // Draw each LED strip as a line
    const ledStrips = solarPanels.reduce((acc, rectangle) => {
      acc.push(Line(rectangle.p1, rectangle.p2));
      acc.push(Line(rectangle.p4, rectangle.p3));
      return acc;
    }, []);

    // Log out the inner and outer coordinates for each LED strip
    let stripNumber = 7;
    ledStrips.forEach(ledStrip => {
      ledStrip.draw(ctx);
      console.log('Strip', stripNumber, ledStrip.log);
      stripNumber = stripNumber == 18 ? 1 : stripNumber + 1;
    });

    // Find the XY coords of a Dot LED
    // given a percentage of how far along the ring it is
    // (percentage or LED number)
    // TODO

    // Emphasize the outer ring using the function (0% and 100%)
    // TODO
  }
};
