"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.ExpandMore = exports.CloseButton = void 0;
var react_1 = __importDefault(require("react"));
var CloseButton = function () { return (react_1.default.createElement("svg", { height: "11", width: "11", viewBox: "0 0 28 28", "aria-hidden": 'false', focusable: false, className: "dv-svg" },
    react_1.default.createElement("path", { d: "M2.1 27.3L0 25.2L11.55 13.65L0 2.1L2.1 0L13.65 11.55L25.2 0L27.3 2.1L15.75 13.65L27.3 25.2L25.2 27.3L13.65 15.75L2.1 27.3Z" }))); };
exports.CloseButton = CloseButton;
var ExpandMore = function () {
    return (react_1.default.createElement("svg", { width: "11", height: "11", viewBox: "0 0 24 15", "aria-hidden": 'false', focusable: false, className: "dv-svg" },
        react_1.default.createElement("path", { d: "M12 14.15L0 2.15L2.15 0L12 9.9L21.85 0.0499992L24 2.2L12 14.15Z" })));
};
exports.ExpandMore = ExpandMore;
