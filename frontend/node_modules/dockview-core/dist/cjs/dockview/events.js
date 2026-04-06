"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.DockviewWillShowOverlayLocationEvent = void 0;
var DockviewWillShowOverlayLocationEvent = /** @class */ (function () {
    function DockviewWillShowOverlayLocationEvent(event, options) {
        this.event = event;
        this.options = options;
    }
    Object.defineProperty(DockviewWillShowOverlayLocationEvent.prototype, "kind", {
        get: function () {
            return this.options.kind;
        },
        enumerable: false,
        configurable: true
    });
    Object.defineProperty(DockviewWillShowOverlayLocationEvent.prototype, "nativeEvent", {
        get: function () {
            return this.event.nativeEvent;
        },
        enumerable: false,
        configurable: true
    });
    Object.defineProperty(DockviewWillShowOverlayLocationEvent.prototype, "position", {
        get: function () {
            return this.event.position;
        },
        enumerable: false,
        configurable: true
    });
    Object.defineProperty(DockviewWillShowOverlayLocationEvent.prototype, "defaultPrevented", {
        get: function () {
            return this.event.defaultPrevented;
        },
        enumerable: false,
        configurable: true
    });
    Object.defineProperty(DockviewWillShowOverlayLocationEvent.prototype, "panel", {
        get: function () {
            return this.options.panel;
        },
        enumerable: false,
        configurable: true
    });
    Object.defineProperty(DockviewWillShowOverlayLocationEvent.prototype, "api", {
        get: function () {
            return this.options.api;
        },
        enumerable: false,
        configurable: true
    });
    Object.defineProperty(DockviewWillShowOverlayLocationEvent.prototype, "group", {
        get: function () {
            return this.options.group;
        },
        enumerable: false,
        configurable: true
    });
    DockviewWillShowOverlayLocationEvent.prototype.preventDefault = function () {
        this.event.preventDefault();
    };
    DockviewWillShowOverlayLocationEvent.prototype.getData = function () {
        return this.options.getData();
    };
    return DockviewWillShowOverlayLocationEvent;
}());
exports.DockviewWillShowOverlayLocationEvent = DockviewWillShowOverlayLocationEvent;
